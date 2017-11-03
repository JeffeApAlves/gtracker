#! /bin/bash
#
#  Menu para configuração de Projeto com ESP32 e IDF
#  As paths do git e comandos estão baseados nesse documento http://esp-idf.readthedocs.io/en/latest/get-started/
#
project_path=$(pwd)
idf_path_dest="$HOME/esp-idf"
idf_path_orin="https://github.com/espressif/esp-idf.git"
toolchain_path="$HOME/esp"
toolchain_path_dest="$HOME/esp"
interface_path="/usr/local/share/openocd/scripts/interface"
target_path="/usr/local/share/openocd/scripts/target"
url_espressif_toolchain64="https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz"
makefile="Makefile"
gdbinitfile="$project_path/gdbinit"
menutitle_program="Seleção do executavel"
menutitle_interface="Seleção da interface"
menutitle_target="Seleção do target"
project_name=$(grep -m 1 PROJECT_NAME $project_path/$makefile | sed 's/^.*= //g')
programfile="$project_path/build/$project_name.elf"
interfacefile=""
targetfile=""
host_gdb=$(grep -m 1 target $gdbinitfile | sed 's/^.*remote \|:.*/''/g')
build_path="$project_path/build"
openocd_sh="$project_path/openocd.sh"
ext_program='elf'
ext_interface='cfg'
ext_target='cfg'

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


function show_info()
{
    whiptail --title "Informação " --msgbox "$selection\n$1" 0 0
}

function show_atencao()
{
    whiptail --title "Atenção " --msgbox "$selection\n$1" 0 0
}

function show_description_file()
{
    whiptail --title "Informações do arquivo" --msgbox " \
    Arquivo selecionado
    Nome     : $1
    Diretorio: $2
    \
    " 0 0 0
}

function show_description_sbc()
{
    whiptail --title "Informações do SBC" --msgbox " \
    SBC encontrada
    IP     : $1
    \
    " 0 0 0
}

function select_path_dest()
{
    p=$(whiptail --inputbox "Insira o diretorio de destino" 8 78 $1 --title "Seleção diretório" 3>&1 1>&2 2>&3)
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        if [ -z $p ]; then

            select_path_dest $1
    
        else

            filepath=$p
        fi    
    fi

    return $exitstatus
}

function select_program()
{
    Filebrowser "$menutitle" "$build_path" "$ext_program"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_description_file "$filename" "$filepath"
            programfile=$filepath/$filename
        fi
    else
        exit
    fi
}

function select_interface()
{
    Filebrowser "$menutitle_interface" "$interface_path" "$ext_interface"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_description_file "$filename" "$filepath"
            interfacefile=$filepath/$filename
        fi
    fi
}

function select_target()
{
    Filebrowser "$menutitle_interface" "$target_path" "$ext_target"
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then
        if [ "$selection" == "" ]; then
            echo "Arquivo não selecionado"
        else
            #show_description_file "$filename" "$filepath"
            targetfile=$filepath/$filename
        fi
    fi
}

function start_gdb()
{
    if [ -f $1 ] ; then
        xtensa-esp32-elf-gdb -x $gdbinitfile $program_name
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

function build_all()
{
    cd $project_path
    make clean
    make all
}

function build_app()
{
    cd $project_path
    make app
}

function create_project()
{
    #TODO
    ls
}

function esp32_flash()
{
    cd $project_path
    
    sudo chmod 777 /dev/ttyUSB0
    make flash 
}

function esp32_monitor()
{
    cd $project_path
    
    sudo chmod 777 /dev/ttyUSB0
    make monitor
}

function esp32_config_screen()
{
    cd $project_path

    make menuconfig
}

function scan_host()
{
    host_gdb=$(sudo nmap -sn 192.168.0.0/24 | awk '/^Nmap/{ip=$NF}/B8:27:EB/{print ip}') > /dev/null
    pid_scan=$?
    show_description_sbc "$host_gdb"
}

function manage_openocd()
{
    #TODO
    make ./boostrap
    make build
    make install
}

function manage_toolchain()
{
    select_path_dest "$toolchain_path_dest"
    exitstatus=$?
                
    if [ $exitstatus -eq 0 ]; then

        toolchain_path_dest=$filepath

        wget "$url_espressif_toolchain64" -O "$HOME/Downloads/tc"
        
        mkdir -p $toolchain_path_dest
        
        cd $toolchain_path_dest

        tar -xzf "$HOME/Downloads/tc"

        toolchain_path=$(find $toolchain_path_dest -name 'xtensa*gcc' | sed 's|/[^/]*$||' )


        #TODO atualizacao profile

        teste=$(grep -m 1 PATH "$HOME/.bash_profile")

        echo $teste

        if [ -z $teste ]; then

            # Atualizar profile 

            export PATH="$PATH:$toolchain_path"

            # TODO add/update no ~/.profile

            echo "OK"
        fi

        echo $PATH

        whiptail --title "Atenção " \
                --msgbox "$selection\nFazer logoff do usuario para atualizar as variáveis de ambiente" 0 0
    fi
}

function update_idf_repositorio()
{
    cd $idf_path_dest

    teste=$(ls | wc -l) > /dev/null
    
    if [ $teste = 0 ]; then

        show_info "Realizando clonagem do repositorio"
        #git clone --recursive $idf_path_orin
        msg="Realizado o clone"

    else

        UPSTREAM=${1:-'@{u}'}
        LOCAL=$(git rev-parse @)
        REMOTE=$(git rev-parse "$UPSTREAM")
        BASE=$(git merge-base @ "$UPSTREAM")

        if [ $LOCAL = $REMOTE ]; then
            msg="Ja se encontrava atualizado\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"

        elif [ $LOCAL = $BASE ]; then
            git pull
            msg="Realizado o pull\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"

        elif [ $REMOTE = $BASE ]; then
            git pull
            msg="Realizado o pull\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"

        else
            msg="Diverge\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
        fi
    fi

    # TODO add/update no ~/.profile
    export IDF_PATH=$idf_path_dest

    show_atencao "Fazer logoff do usuario para atualizar das variáveis de ambiente\n\nRepositorio:$msg"
}

function manage_idf()
{
    if [ ${#IDF_PATH} -ne 0 ]; then
            
        idf_path_dest=$IDF_PATH
    fi

    while : ; do

        select_path_dest "$idf_path_dest"

        exitstatus=$?
            
        if [ $exitstatus = 0 ]; then

            idf_path_dest=$filepath

            if [ ! -d "$idf_path_dest" ]; then
                
                if mkdir $idf_path_dest ; then

                    update_idf_repositorio
                    break
                fi

            else
          
                update_idf_repositorio
                break
            fi
        else
            break
        fi
    done
}

function install_dependencias()
{
    sudo apt-get install git wget make libncurses-dev flex bison gperf python python-serial
    sudo apt-get install git
}

debug_screen()
{
    clear

    while : ; do

        CHOICE=$(
                whiptail --title "Projeto" --menu "Selecione uma das opções abaixo" 0 0 0 \
                    "[1]" "Executar server(openocd)"  \
                    "[2]" "Executar debug (prompt)"   \
                    "[3]" "Interface (adaptador)"  \
                    "[3]" "Target (device)"  \
                    "[5]" "Monitor"  \
                    "[6]" "Scan server"   3>&2 2>&1 1>&3
                )

        exitstatus=$?

        if [ $exitstatus = 0 ]; then

            case $CHOICE in

                "[1]") start_debug_server ;;
                "[2]") start_section ;;
                "[3]") setup_interface ;;
                "[4]") setup_target ;;
                "[5]") esp32_monitor ;;
                "[6]") scan_host ;;
            esac
        else
            break
        fi
    done
}

project_screen()
{
    clear

    while : ; do

        CHOICE=$(
                whiptail --title "Projeto" --menu "Selecione uma das opções abaixo" 0 0 0 \
                    "[1]" "Compilar todo o projeto"   \
                    "[2]" "Compilar app"  \
                    "[3]" "Criar projeto" 3>&2 2>&1 1>&3
                )

        exitstatus=$?

        if [ $exitstatus = 0 ]; then

            case $CHOICE in

                "[1]") build_all ;;
                "[2]") build_app ;;
                "[3]") create_project ;;
            esac
        else
            break
        fi
    done
}

enviroment_screen()
{
    clear

    while : ; do

        CHOICE=$(
                whiptail --title "Ambiente de desenvolvimento" --menu "Selecione uma das opções abaixo" 0 0 0 \
                    "[1]" "Instalar/atualizar ESP-IDF"   \
                    "[2]" "Instalar/atualizar openocd"  \
                    "[3]" "Instalar/atualizar toolchain" 3>&2 2>&1 1>&3
                )

        exitstatus=$?

        if [ $exitstatus = 0 ]; then

            case $CHOICE in

                "[1]") manage_idf ;;
                "[2]") manage_openocd ;;
                "[3]") manage_toolchain ;;
            esac
        else
            break
        fi
    done
}

esp32_screen()
{
    clear

    while : ; do

        CHOICE=$(
                whiptail --title "Ambiente de desenvolvimento" --menu "Selecione uma das opções abaixo" 0 0 0 \
                    "[1]" "Gravar (uart)"   \
                    "[2]" "Monitor (uart)"  \
                    "[3]" "Configuração"  3>&2 2>&1 1>&3
                )
        exitstatus=$?

        if [ $exitstatus = 0 ]; then

            case $CHOICE in

                "[1]") esp32_flash ;;
                "[2]") esp32_monitor ;;
                "[3]") esp32_config_screen ;;
            esac
        else
            break
        fi
    done
}

function main_screen()
{
    clear

    while : ; do

        CHOICE=$(
                whiptail --title "Principal" --menu "Selecione uma das opções abaixo" 0 0 0 \
                    "[1]" "Debug"   \
                    "[2]" "Projeto" \
                    "[3]" "ESP32" \
                    "[4]" "Ambiente de desenvolvimento" 3>&2 2>&1 1>&3
                )

        exitstatus=$?

        if [ $exitstatus = 0 ]; then

            case $CHOICE in

                "[1]") debug_screen ;;
                "[2]") project_screen ;;
                "[3]") esp32_screen ;;
                "[4]") enviroment_screen ;;
            esac
        else
            break
        fi
    done

    exit
}

main_screen