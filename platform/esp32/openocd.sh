#! /bin/bash
#
# Inicia o servidor de debug na RBPI zero reseta o device atrave da manipulacao de GPIO
# Arquivo de configuração da interface usr/local/share/openocd/scripts/interface/raspberrypi-native.cfg
#

process_name="openocd"
target="target/esp32.cfg" 
interface="interface/raspberrypi-native.cfg"
transport="transport select jtag"
url_openocd="git://git.code.sf.net/p/openocd/code openocd-code"
openocd_path="$HOME/openocd-code"


function show_info() {

    dialog --title "Informações" --infobox "$1" 0 0
    sleep 3
    clear
}

function close_openocd(){

  sudo pkill $process_name

  openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  echo $openocd_pid

  #if [ $openocd_pid ]; then

    #sudo su - -c echo kill $openocd_pid
    
    #ps -ef | grep openocd | grep -v grep | awk '{print $2}' | xargs echo kill

    #openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  #fi
}

function start_openocd(){

  openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  echo "Interface: $interface Targer: $target"
  if [ $openocd_pid ]; then
    echo "$process_name ja estava em execução PID: $openocd_pid"
  else
    sudo openocd -f $1 -c "transport select jtag" -f $2 -d 3 2>&1 | tee openocd.log  
    openocd_pid=$(ps -ef | grep $process_name | grep -v 'grep' | awk '{ printf $2 }')
    echo "$process_name em execução PID: $openocd_pid"
  fi
}

function reset_target() {

  gpio -g mode 8 out
  gpio -g write 8 1
  sleep 5
  gpio -g write 8 0
  sleep 0.3
  gpio -g write 8 1
}

function install_dependencias() {

    sudo apt-get -y install git wget make libncurses-dev flex bison gperf python python-serial  minicom autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev &> /tmp/install.log &

    dialog --title "Instalação dos pacotes" --tailbox  /tmp/install.log 30 100
}

function clone_repositorio() {

    local origem=$1
    local destino=$2

    cd $destino &&

    git clone --recursive --progress $origem 2>&1 | \
    stdbuf -o0 awk 'BEGIN{RS="\r|\n|\r\n|\n\r";ORS="\n"}/Receiving/{print substr($3, 1, length($3)-1)}' | \
    dialog --title "Download" --gauge "Por favor espere. Clonagem em andamento.\n\nDe  :$origem\nPara:$destino" 0 0 0
}

function update_repositorio() {
    #Opcional passar o branch em $2" ex:release/v2.1"

    local destino=$1

    cd $destino &&

    git remote update> /dev/null &&
    git status -uno > /dev/null &&

    UPSTREAM=${2:-'@{u}'} &&
    LOCAL=$(git rev-parse @) && 
    REMOTE=$(git rev-parse "$UPSTREAM") &&
    BASE=$(git merge-base @ "$UPSTREAM") &&

    if [ $LOCAL = $REMOTE ]; then
        show_info "Repositorio:\nAtualizado !\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    elif [ $LOCAL = $BASE ]; then
        show_info "Repositorio:\nAtaulização iniciada\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
        git pull
        git submodule update
        #pid=$?
    elif [ $REMOTE = $BASE ]; then
        #ait $pid

        show_info "Repositorio:\nAtaulização iniciada\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
        git pull
        git submodule update
        #pid=$?
        #wait $pid
    else
        show_info "Repositorio:\nDiverge\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    fi
}

function get_source_code() {

  if [[ -d "$openocd_path" || $(mkdir $openocd_path) = 0 ]]; then

      { 
        update_repositorio "$openocd_path" 
      } || { 
          
        clone_repositorio "$url_openocd" "$openocd_path" 
      } || {

        show_info "Não foi possivel clonar/atualizar o OPENOCD"
      }
  fi
}


function install_openocd() {

  install_dependencias

  get_source_code

  ./bootstrap

  ./configure --enable-sysfsgpio --enable-bcm2835gpio

  make
  
  sudo make install
}

function init_script() {
    # startup do script

    sudo apt-get install -y -qq  dialog

    #limpa a tela
    clear
}

init_script

#reset_target
close_openocd

#reset_target &

start_openocd "$interface" "$target"

echo "Servidor iniciado"