#! /bin/bash
#
#  Existe um segundo script "openocd.sh" que será rodado no server openocd (raspberry)
#
#                  deploy: sincroniza\atualiza arquivos  no webserver
# [opções]
#           -i <interface> Nome do arquivo correpsonente a interafce que sera usado no debug
#           -t <target> Nome do arquivo correpsonde ao target que sera debugado
#           -u <url> Url do repositorio do openocd para instala;
#

# Diretorio onde se encontra o script
BASEDIR="${0%/*}"

source $BASEDIR/def.sh
source $BASEDIR/misc.sh

url_openocd="https://github.com/espressif/openocd-esp32.git"
interface_path="/usr/local/share/openocd/scripts/interface"
target_path="/usr/local/share/openocd/scripts/target"
openocdfile="$PROJECT_HOME/tools/openocd.sh"
gdbinit="$project_path/gdbinit"
target="$target_path/esp32.cfg" 
interface="$interface_path/raspberrypi-native.cfg"
program="$project_path/build/$project_name.elf"
host_gdb=$(grep -m 1 target $gdbinit | sed 's/^.*remote \|:.*/''/g')
ext_program='elf'
ext_config='cfg'
freq_jtag=10000


function select_program() {

    select_file "Selecione o programa" "$build_path" "$ext_program"

    local RET=$?

    if [ $RET -eq 0 ]; then

        program="$filepath/$filename"
        RET=0
    fi

    return $RET
}

function select_interface(){

    select_file "Selecione o adaptador (interface)" "$interface_path" "$ext_config" "REMOTE"

    local RET=$?

    if [ $RET -eq 0 ]; then

        interface="$filepath/$filename"
        RET=0
    fi

    return $RET
}

function select_target() {

    select_file "Selecione o target" "$target_path" "$ext_config" "REMOTE"

    local RET=$?

    if [ $RET -eq 0 ]; then

        target="$filepath/$filename"
        RET=0
    fi

    return $RET
}

function start_gdb() {

    local init=$1
    local program=$2

    if [ -f $init ] ; then
        if [ -f $program ] ; then
            clear
            xtensa-esp32-elf-gdb -x $init $program
        else
            show_msgbox "ERRO!" "Não foi possivel localizar o arquivo:\n$program"
        fi
    else
        show_msgbox "ERRO!" "Não foi possivel localizar o arquivo:\n$init"
    fi
}

function start_debug_server() {

    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "start"  -i "$interface" -t "$target" &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$USER iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function stop_debug_server() {

    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "stop"  &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$USER iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function start_section_debug() {

    select_program
    local RET=$?

    if [ $RET -eq 0 ]; then

        start_gdb "$gdbinit" "$program"
    fi
}

function config_jtag() {

    freq_jtag=$(dialog --stdout \
                --title "Nome do projeto" \
                --backtitle "$app_title" \
                --inputbox "Informe a frequencia do jtag em [KHz]" 10 100 $freq_jtag \
            )

    local RET=$?

    if [ $RET -ne 1 ]; then

        ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "config" -i "$interface" -f "$freq_jtag" &> /dev/null

#        dialog \
#            --no-shadow \
#            --title "$USER iniciando debug no host IP:$host_gdb" \
#            --tailbox /tmp/ssh.log 40 100
    fi
}

function install_openocd() {

    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "install" -u $url_openocd &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$USER iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function shutdown_interface() {

    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "shutdown" &> /tmp/ssh.log  &

    dialog \
        --no-shadow \
        --title "$USER iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function reset_target() {

    ssh $USER@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "reset" &> /tmp/ssh.log  &

    dialog \
        --no-shadow \
        --title "$USER iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function scan_gdbserver() {

    host_gdb=$(sudo nmap -sn 192.168.0.0/24 | awk '/^Nmap/{ip=$NF}/B8:27:EB/{print ip}')

    show_description_sbc "$host_gdb"
}