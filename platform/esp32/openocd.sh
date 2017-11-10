#! /bin/bash
#
# Inicia o servidor de debug na RBPI zero reseta o device atrave da manipulacao de GPIO
#
# Observação: O arquivo de configuração da interface usr/local/share/openocd/scripts/interface/raspberrypi-native.cfg
# Nao esquecer de configurar no aqrquivo target o clock e o rst
# http://esp-idf.readthedocs.io/en/latest/api-guides/jtag-debugging/tips-and-quirks.html#jtag-debugging-tip-openocd-configure-target

interface_path="/usr/local/share/openocd/scripts/interface"
target_path="/usr/local/share/openocd/scripts/target"
process_name="openocd"
interface="raspberrypi-native.cfg"
target="esp32.cfg"  
transport="jtag"
url_openocd="https://github.com/espressif/openocd-esp32.git"
openocd_path="$HOME/openocd-code"
cmd=""
freq_jtag=400

function stop_server(){
  # Finaliza o processo correspondente ao openocd

  sudo pkill $process_name
}

function start_openocd(){
  # Inicia o openocd utilizando os parametro recebidos

  sudo openocd -f $1 -c "transport select $transport" -f $2 -d 3
}

function reset_target() {
  #Manuipula o GPIO que esta ligado ao pino de resete do ESP32

  gpio -g mode 8 out
  gpio -g write 8 1
  sleep 2             # tempo para inicializar o modo jtag
  gpio -g write 8 0
  sleep 0.3
  gpio -g write 8 1
  sleep 1             # Tempo para startup
}

function start_server(){
  # incia o servidor de debug

  # reseta o target atrabves de um GPIO ligado ao reset
  reset_target &

  start_openocd "$interface" "$target"
}

function install_dependencias() {
    # Instala os pacotes necessarios para execução do server debug OpenOCD

    sudo apt-get -y install \
      git \
      wget \
      make \
      libncurses-dev \
      flex \
      bison \
      gperf \
      python \
      python-serial \
      minicom \
      autoconf \
      libtool \
      pkg-config \
      libusb-1.0-0 \
      libusb-1.0-0-dev
}

function clone_repositorio() {
    # realiza de um repositorio no git

    local origem=$1
    local destino=$2

    git clone --recursive $origem $destino
}

function update_repositorio() {
    # atualiza o repositorio local
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
  # Faz a copia ou atualiza o codigo fonte do openocd para compilação

  if [[ -d "$openocd_path" || $(mkdir $openocd_path) -eq 0 ]]; then

      { 
        echo "Atualizando source-code"
        update_repositorio "$openocd_path" 
      } || { 

        echo "Clonando source-code"  
        clone_repositorio "$url_openocd" "$openocd_path"
      } || {

        echo "Não foi possivel clonar/atualizar o OpenOCD"
      }
  else
    echo "Duretorio Não foi possivel clonar/atualizar o OpenOCD"
  fi
}

function install_openocd() {
  # Instala o OpenOCD

  install_dependencias &&

  get_source_code &&

  cd $openocd_path

  ./bootstrap &&

  ./configure --enable-sysfsgpio --enable-bcm2835gpio &&

  make &&
  
  sudo make install

  echo "Fim do script de instalação"
}

function config_jtag() {

  sed -e '/adapter_khz/D' "$interface" > "/tmp/interface"
  echo  'adapter_khz '$freq_jtag >> "/tmp/interface"
  sudo  cp "/tmp/interface" "$interface"
}

##### Main - Entrada do script

clear

while getopts "c:i:t:u:f:" opt; do

  case $opt in
    "u")  url_openocd=$OPTARG ;;
    "c")  cmd=$OPTARG         ;;
    "i")  interface=$OPTARG   ;;
    "t")  target=$OPTARG      ;;
    "f")  freq_jtag=$OPTARG   ;; 
    "?")  exit -2             ;;
  esac

done

if [ $cmd = "install" ]; then

  install_openocd

elif  [ $cmd = "shutdow" ]; then

  sudo shutdown now

elif  [ $cmd = "start" ]; then

  stop_server

  start_server

elif  [ $cmd = "stop" ]; then

  stop_server

elif  [ $cmd = "reset" ]; then

  reset_target

elif  [ $cmd = "config" ]; then

  config_jtag

else
  exit -2
fi 
