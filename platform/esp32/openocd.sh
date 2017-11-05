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

function close_openocd(){
  
  openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  if [ $openocd_pid ]; then

    #sudo su - -c echo kill $openocd_pid
    
    ps -ef | grep openocd | grep -v grep | awk '{print $2}' | xargs echo kill

    openocd_pid=$(ps -ef | grep openocd | grep -v 'grep' | awk '{ printf $2 }')

  fi
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

    sudo apt-get -y install git wget make libncurses-dev flex bison gperf python python-serial  minicom autoconf libtool make pkg-config libusb-1.0-0 libusb-1.0-0-dev
}

function get_source_code() {

  mkdir $openocd_path
  cd $openocd_path
  git clone --recursive  $url_openocd
}

function install_openocd() {

  install_dependencias

  git clone --recursive  $url_openocd

  ./bootstrap

  ./configure --enable-sysfsgpio --enable-bcm2835gpio

  make
  
  sudo make install
}

function start_server() {
  #reset_target
  close_openocd
  reset_target &
  start_openocd "$interface" "$target"
  echo "Target reiniciado"
}

start_server