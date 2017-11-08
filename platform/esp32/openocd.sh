#! /bin/bash
#
# Inicia o servidor de debug na RBPI zero reseta o device atrave da manipulacao de GPIO
# Arquivo de configuração da interface usr/local/share/openocd/scripts/interface/raspberrypi-native.cfg
#

project_path=$(pwd)
process_name="openocd"
target="target/esp32.cfg" 
interface="interface/raspberrypi-native.cfg"
transport="transport select jtag"
url_openocd="git://git.code.sf.net/p/openocd/code openocd-code"
openocd_path="$HOME/openocd-code"

function close_openocd(){

  sudo pkill $process_name
}

function start_openocd(){

  #openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  #echo "Interface: $interface Targer: $target"
  #if [ $openocd_pid ]; then
  #  echo "$process_name ja estava em execução PID: $openocd_pid"
 # else
    sudo openocd -f $1 -c "transport select jtag" -f $2 -d 3 2>&1 | tee openocd.log  
    #openocd_pid=$(ps -ef | grep $process_name | grep -v 'grep' | awk '{ printf $2 }')
    #echo "$process_name em execução PID: $openocd_pid"
  #fi
}

function reset_target() {

  gpio -g mode 8 out
  gpio -g write 8 1
  sleep 1
  gpio -g write 8 0
  sleep 0.3
  gpio -g write 8 1
}

function install_dependencias() {

    sudo apt-get -y install git wget make libncurses-dev flex bison gperf python python-serial  minicom autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev
}

function clone_repositorio() {

    local origem=$1
    local destino=$2

    cd $destino &&

    git clone --recursive --progress $origem 2>&1 | \
    stdbuf -o0 awk 'BEGIN{RS="\r|\n|\r\n|\n\r";ORS="\n"}/Receiving/{print substr($3, 1, length($3)-1)}'
}

function update_repositorio() {
    #$2: [opcional] passar o branch  ex:release/v2.1"

    local destino=$1

    cd $destino &&
    git remote update > /dev/null &&
    git status -uno  > /dev/null &&

    UPSTREAM=${2:-'@{u}'} &&
    LOCAL=$(git rev-parse @) && 
    REMOTE=$(git rev-parse "$UPSTREAM") &&
    BASE=$(git merge-base @ "$UPSTREAM") &&

    if [ $LOCAL = $REMOTE ]; then
        echo "Atualizado !\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    elif [ $LOCAL = $BASE ]; then

        git submodule update
        git pull

    elif [ $REMOTE = $BASE ]; then

        git submodule update
        git pull

      else

        echo "Divergencias\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    fi
}

function get_source_code() {

  if [[ -d "$openocd_path" || $(mkdir $openocd_path) = 0 ]]; then

      { 
        update_repositorio "$openocd_path" 
      } || { 
          
        clone_repositorio "$url_openocd" "$openocd_path" 
      } || {

        echo "Não foi possivel clonar/atualizar o OPENOCD"
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

    #limpa a tela
    clear
}

init_script

close_openocd

start_openocd "$interface" "$target"

reset_target