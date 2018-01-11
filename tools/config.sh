#! /bin/bash
#
#  Menus para configuração do projeto ESP32 / SDK IDF / OpenOCD 
#

# Diretorio onde se encontra o script
BASEDIR="${0%/*}"

ource $BASEDIR/misc.sh
source $BASEDIR/gdb.sh

# Home do projeto ESP32
ESP32_PROJECT_HOME=$PROJECT_HOME/platform/esp32

# Tela anterior
BEFORE_SCREEN=null

# Tela atual 
ACTIVATE_SCREEN=null

declare -a  PACKAGES_TO_INSTALL

# Pacotes para instalação
PACKAGES_TO_INSTALL=(git make wget nmap flex bison gperf python python-serial minicom)

function debug_screen() {

    # Tela de debug

    CHOICE=$(dialog --stdout\
                --title "Debug" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo\n\nInterface:$interface\nTarget   :$target" 20 100 20 \
                1 "Iniciar debug (prompt)"   \
                2 "Iniciar server(openocd)"  \
                3 "Parar server(openocd)"  \
                4 "Interface (adaptador)"  \
                5 "Target (device)"  \
                6 "JTAG"  \
                7 "Reset target" \
                8 "Scan server" \
                9 "Desligar interface"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) start_section_debug ;;
                2) start_debug_server ;;
                3) stop_debug_server ;;
                4) select_interface ;;
                5) select_target ;;
                6) config_jtag ;;
                7) reset_target ;;
                8) scan_gdbserver ;;
                9) shutdown_interface ;;
        esac
    fi

    return $RET
}

function project_screen() {

    # Tela gerenciamento do projeto

    CHOICE=$(dialog --stdout \
                --title "Projeto" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Compilar all"   \
                2 "Compilar app"  \
                3 "Criar projeto"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) $BASEDIR/esp32.sh build -d $ESP32_PROJECT_HOME -s all ;;
                2) $BASEDIR/esp32.sh build -d $ESP32_PROJECT_HOME -s app ;;
                3) $BASEDIR/esp32.sh create -d $ESP32_PROJECT_HOME ;;
        esac
    fi

    return $RET
}

function enviroment_screen() {

    # Tela para configuração do ambiente de desenvolvimento

    CHOICE=$(dialog --stdout \
                --title "Ambiente de desenvolvimento" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Instalar/atualizar ESP-IDF"   \
                2 "Instalar/atualizar openocd"  \
                3 "Instalar/atualizar toolchain"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) $BASEDIR/esp32.sh install -d $ESP32_PROJECT_HOME -s sdk ;;
                2) install_openocd ;;
                3) $BASEDIR/esp32.sh install -d $ESP32_PROJECT_HOME -s toolchain ;;
        esac
    fi

    return $RET
}

function esp32_screen() {

    # Tela de configuração do ESP32

    CHOICE=$(dialog --stdout \
                --title "ESP32" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Gravar (uart)"   \
                2 "Monitor (uart)"  \
                3 "Configuração"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) $BASEDIR/esp32.sh esp -d $ESP32_PROJECT_HOME -s flash ;;
                2) $BASEDIR/esp32.sh esp -d $ESP32_PROJECT_HOME -s monitor ;;
                3) $BASEDIR/esp32.sh esp -d $ESP32_PROJECT_HOME -s config ;;
        esac
    fi

    return $RET
}

function main_screen() {

    # Tela principal

    CHOICE=$(dialog --stdout \
                --title "Principal"\
                --backtitle "$app_title" \
                --no-tags --scrollbar \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Debug" \
                2 "Projeto" \
                3 "ESP32" \
                4 "Ambiente de desenvolvimento" 
             )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) show_screen debug_screen ;;
                2) show_screen project_screen ;;
                3) show_screen esp32_screen ;;
                4) show_screen enviroment_screen ;;
        esac
    fi

    return $RET
}

function show_screen() {
    # selecioan um menu para ser mostrado na tela

    BEFORE_SCREEN=$ACTIVATE_SCREEN

    ACTIVATE_SCREEN=$1
}

function install_packages() {

    echo "Por favor espere..."

    local installed=$(dpkg --get-selections)

	for pkg in ${PACKAGES_TO_INSTALL[*]}; do

        
        status=$(dpkg -s "$pkg" | grep "install ok installed") > /dev/null
        
        if [ ! -n "$status" ] ; then

            sudo apt-get install -qq $pkg 

            #sudo apt-get -y install "$pkg"  &> /tmp/install.log &
            #dialog --title "Instalação do pacote" --tailbox  /tmp/install.log 30 100
        fi		

	done
}

function main_loop(){

    while : ; do

        $ACTIVATE_SCREEN;
        RET=$?

        # processa eventos com hanldes comunus
        if [ $RET -ne 0 ]; then

            if [ $ACTIVATE_SCREEN = main_screen ]; then
                # ESC na tela principal entao sai do script
                break
            else
                show_screen $BEFORE_SCREEN
            fi
        fi
    done
}

### Entrada do script

# startup do script

# instala os pacote que ainda não estão instalados
install_packages

#inicializa com o menu principal
show_screen main_screen

# loop infinito 
main_loop

clear