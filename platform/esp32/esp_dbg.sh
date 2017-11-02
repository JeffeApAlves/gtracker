#! /bin/bash
#
#  Menu para configuração de Projeto com ESP32 e IDF
#

project_dir=$(pwd)
interface_dir=/usr/local/share/openocd/scripts/interface/
target_dir=/usr/local/share/openocd/scripts/target/
gdb_init=$project_dir/gdbinit
makefile="Makefile"
prj_name=$(grep -m 1 PROJECT_NAME $project_dir/$makefile | sed 's/^.*= //g')
file_program=$project_dir/build/$prj_name.elf
file_interface=""
file_target=""
host_gdb=$(grep -m 1 target $gdb_init | sed 's/^.*remote \|:.*/''/g')
process_name="openocd"
openocd_sh=$project_dir/openocd.sh

menutitle_program="Seleção do executavel"
menutitle_interface="Seleção da interface"
menutitle_target="Seleção do target"

ext_program='elf'
ext_interface='cfg'
ext_target='cfg'

export PATH=$PATH:$HOME/esp/xtensa-esp32-elf/bin
export IDF_PATH=/media/$USER/Dados/workspace/esp-idf

function Filebrowser()
{
    if [ -z $2 ] ; then
        dir_list=$(ls -lhp  | awk -F ' ' ' { print $9 " " $5 } ')
    else
        cd "$2"
        dir_list=$(ls -lhp  | awk -F ' ' ' { print $9 " " $5 } ')
    fi

    curdir=$(pwd)
    if [ "$curdir" == "/" ] ; then  # verifica se é o diretorio raiz
        selection=$(whiptail --title "$1" \
                              --menu "Arquivo para debug\n$curdir" 0 5 0 \
                              --cancel-button Cancel \
                              --ok-button Select $dir_list 3>&1 1>&2 2>&3)
    else 
        selection=$(whiptail --title "$1" \
                              --menu "Arquivo para debug\n$curdir" 0 0 0 \
                              --cancel-button Cancel \
                              --ok-button Select ../ Voltar $dir_list 3>&1 1>&2 2>&3)
    fi

    RET=$?
    if [ $RET -eq 1 ]; then 
       return 1
    elif [ $RET -eq 0 ]; then
       if [[ -d "$selection" ]]; then 
          Filebrowser "$1" "$selection"
       elif [[ -f "$selection" ]]; then 
          if [[ $selection == *$3 ]]; then # verifica a exxtensão 
            if (whiptail --title "Confirm Selection" --yesno "DirPath : $curdir\nFileName: $selection" 0 0 \
                         --yes-button "Confirm" \
                         --no-button "Retry"); then
                filename="$selection"
                filepath="$curdir"
            else
                Filebrowser "$1" "$curdir"
            fi
          else 
             whiptail --title "ERROR: Arquivo deve ter a extensão  $3 " \
                      --msgbox "$selection\nVoce deve selecionar um arquivo do tipo $3" 0 0
             Filebrowser "$1" "$curdir"
          fi
       else
          # Não foi possivel ler o arquivo
          whiptail --title "ERROR: Erro na seleção" \
                   --msgbox "Error alteração do caminho $selection" 0 0
          Filebrowser "$1" "$curdir"
       fi
    fi
}

function show_msg_file()
{
    whiptail --title "Informações do arquivo" --msgbox " \
    Arquivo selecionado
    Nome     : $1
    Diretorio: $2
    \
    " 0 0 0
}

function select_program()
{
    Filebrowser "$menutitle" "$project_dir/build" "$ext_program"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_msg_file "$filename" "$filepath"
            file_program=$filepath/$filename
        fi
    else
        exit
    fi
}

function select_interface()
{
    Filebrowser "$menutitle_interface" "$interface_dir" "$ext_interface"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_msg_file "$filename" "$filepath"
            file_interface=$filepath/$filename
        fi
    fi
}

function select_target()
{
    Filebrowser "$menutitle_interface" "$target_dir" "$ext_target"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_msg_file "$filename" "$filepath"
            file_target=$filepath/$filename
        fi
    fi
}

function start_gdb()
{
    if [ -f $1 ] ; then
        xtensa-esp32-elf-gdb -x $gdb_init $program_name
    else
        echo "Não foi possivel localizar o arquivo de debug:$1"
    fi
}

function start_debug_server()
{
    user=$(whoami)
    echo "$user iniciando debug no host IP:$host_gdb"
    ssh $user@$host_gdb 'sudo -Sv && bash -s' < $openocd_sh
}

function start_section()
{
    start_gdb "$program_name"
}

function setup_interface()
{
    select_interface
}

function setup_target()
{
    select_target
}

function compilar()
{
    cd $IDF_PATH
    git  pull
    cd $project_dir
    make clean
    make all
}

function flash()
{
    sudo chmod 777 /dev/ttyUSB0

    make flash 
}

function monitor()
{
    sudo chmod 777 /dev/ttyUSB0

    make monitor
}

function esp32_config()
{
    make menuconfig
}

function main_menu()
{
    clear

    while [ 1 ]
    do
        CHOICE=$(
        whiptail --title "Menu principal" --menu "Selecione uma das opções abaixo" 0 0 0 \
            "1)" "Iniciar servidor de debug (openocd)"   \
            "2)" "Iniciar debugguer (gdb)"  \
            "3)" "Interface de debug" \
            "4)" "Target para debug" \
            "5)" "Monitor (uart)" \
            "6)" "Gravaçao FW (uart)" \
            "7)" "Compilar projeto" \
            "8)" "Configuração ESP32" \
            "9)" "Sair"  3>&2 2>&1 1>&3	
        )

        case $CHOICE in

            "1)") start_debug_server ;;
            "2)") start_section ;;
            "3)") setup_interface ;;
            "4)") setup_target ;;
            "5)") monitor ;;
            "6)") flash ;;
            "7)") compilar ;;
            "8)") esp32_config ;;
            "9)") exit ;;
        esac
    done
}

main_menu

