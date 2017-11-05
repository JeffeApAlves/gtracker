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
url_espressif_toolchain32="https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-61-gab8375a-5.2.0.tar.gz"
makefile="$project_path/Makefile"
gdbinitfile="$project_path/gdbinit"
project_name=$(grep -m 1 PROJECT_NAME $makefile | sed 's/^.*= //g')
programfile="$project_path/build/$project_name.elf"
interfacefile=""
targetfile=""
host_gdb=$(grep -m 1 target $gdbinitfile | sed 's/^.*remote \|:.*/''/g')
build_path="$project_path/build"
openocdfile="$project_path/openocd.sh"
ext_program='elf'
ext_config='cfg'
activate_screen=0
template_project="$IDF_PATH/examples/get-started/blink ."

function select_file() {

    if [ ! -z $2 ] ; then
        cd "$2"
    fi

    # lista diretorio e arquivos filtrados pela extensão
    dir_list=$(ls -lhd */  *.$3 | awk -F ' ' ' { print $9 " " $5 } ')

    curdir=$(pwd)

    if [ "$curdir" != "/" ] ; then
        dir_list="../ Voltar $dir_list" 
    fi

    selection=$(
        whiptail --fb --title "Seleção arquivo" \
            --menu "$1\n$curdir" 45 100 30 \
            --cancel-button Cancelar \
            --ok-button Selecionar $dir_list 3>&1 1>&2 2>&3
        )
    RET=$?

    if [ $RET -eq 0 ]; then

        if [[ -d "$selection" ]]; then 
            
            select_file "$1" "$selection" "$3"
        
        elif [[ -f "$selection" ]]; then 
 
            if [[ $selection == *$3 ]]; then # verifica a exxtensão 
                
                if (whiptail --fb --title "Confirmação da seleção" --yesno "Diretorio: $curdir\nArquivo  : $selection" 10 100 \
                            --yes-button "Confirmar" \
                            --no-button "Voltar"); then
                    
                    filename="$selection"
                    filepath="$curdir"

                else
                    select_file "$1" "$curdir" "$3"
                fi
            else
                show_erro "Arquivo incompativel" "$selection\nVoce deve selecionar um arquivo do tipo $3"
                select_file "$1" "$curdir" "$3"
            fi
 
        else # Não foi possivel ler o arquivo
            show_erro "Caminho ou arquivo invalido" "Não foi possivel acessa-lo:$selection"
            select_file "$1" "$curdir" "$3"
        fi
    fi

    return $RET
}

function select_path() {

    if [ ! -z $2 ] ; then
        cd "$2"
    fi

    dir_list=$(ls -lhd */ | awk -F ' ' ' { print $9 " " $5 } ')

    curdir=$(pwd)

    if [ "$curdir" != "/" ] ; then
        dir_list="../ Voltar $dir_list" 
    fi

    selection=$(
        whiptail --fb --title "Seleção pasta" \
            --menu "$1\n$curdir" 45 100 30 \
            --cancel-button "Confirmar" \
            --ok-button "Selecionar" $dir_list 3>&1 1>&2 2>&3
        )
    RET=$?

    if [ $RET -eq 0 ]; then

        select_path "$1" "$selection"

    elif [ $RET -eq 1 ]; then #O botao cancel foi transforma em OK  durante a seleção de uma path
 
        if (whiptail --fb --title "Confirmação da seleção" --yesno "Diretório:$curdir" 10 100 \
                    --yes-button "Confirmar" \
                    --no-button "Voltar"); then
            
            filepath="$curdir"
            return 0

        else
            select_path "$1" "$curdir"
        fi
    fi

    return $RET
}

function show_erro() {

    whiptail --fb --title "ERROR:$1 " --msgbox "$2" 0 0
}

function show_info() {

    echo "$1"
    #TDODO usar whiptail
    whiptail --title "Informações" --infobox "$1" 0 0
    sleep 1
    #clear
}

function show_atencao() {

    whiptail --fb --title "ATENÇÂO" --msgbox "$1" 0 0
}

function show_description_file() {

    whiptail --fb --title "Informações do arquivo" --msgbox " \
    Arquivo selecionado
    Nome     : $1
    Diretorio: $2
    \
    " 0 0 0
}

function show_description_sbc() {

    whiptail --fb --title "Informações do SBC" --msgbox \
    "SBC encontrada\nIP: $1 " \
    0 0 0
}

function select_program() {

    select_file "Selecione o programa" "$build_path" "$ext_program"
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        programfile=$filepath/$filename
    fi

    return $exitstatus
}

function select_interface(){

    select_file "Selecione o adaptador (interface)" "$interface_path" "$ext_config"
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        interfacefile=$filepath/$filename
    fi

    return $exitstatus
}

function select_target() {

    select_file "Selecione o target" "$target_path" "$ext_config"
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        targetfile=$filepath/$filename
    fi

    return $exitstatus
}

function start_gdb() {

    cd $project_path

    if [ -f $1 ] ; then
        xtensa-esp32-elf-gdb -x $gdbinitfile $1
    else
        show_atencao "Não foi possivel localizar o arquivo de debug:$1"
    fi
}

function start_debug_server() {

    user=$(whoami)
    echo "$user iniciando debug no host IP:$host_gdb"
    ssh $user@$host_gdb 'sudo -Sv && bash -s' < $openocdfile
}

function start_section_debug() {

    select_program
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        start_gdb "$programfile"
    fi
}

function setup_interface() {

    select_interface
}

function setup_target() {

    select_target
}

function build_all() {

    cd $project_path
    make clean
    make all
}

function build_app() {

    cd $project_path
    make app
}

function create_project() {

    new_project_name=$(
            whiptail --title "Nome do projeto" \
                --fb \
                --inputbox "Informe o nome do projeto" 10 100 project_name 3>&2 2>&1 1>&3
            )

    exitstatus=$?
                
    if [ $exitstatus -eq 0 ]; then

        select_path "Seleção destino projeto" "$HOME"
        exitstatus=$?
                    
        if [ $exitstatus -eq 0 ]; then

            cd $filepath
            cp -r $template_project
            mv "blink" $new_project_name

            cp "$project_path/esp_dbg.sh" "$filepath/$new_project_name"
            cp "$project_path/openocd.sh" "$filepath/$new_project_name"

            #TODO renomear propriedade arquivo
        fi
    fi
}

function esp32_flash() {

    cd $project_path

    whiptail --fb --textbox /dev/stdin 40 80 <<<"$(make flash)" 
}

function esp32_monitor() {

    cd $project_path

    make monitor
}

function esp32_config_screen() {

    cd $project_path

    make menuconfig
}   

function scan_host() {

    host_gdb=$(sudo nmap -sn 192.168.0.0/24 | awk '/^Nmap/{ip=$NF}/B8:27:EB/{print ip}')
    #pid=$?

    #wait $pid
    show_description_sbc "$host_gdb"
}

function manage_openocd() {

    #TODO
    ls
}

function download_file() {

    wget "$1" -O "$2"  2>&1 | \
    stdbuf -o0 awk '/[.] +[0-9][0-9]?[0-9]?%/ { print substr($0,63,3) }' | \
    whiptail --title "Download" --gauge --cancel-button "Cancelar"  "Por favor espere download em andamento\nDe  :$1\nPara:$2" 0 0 0
}

function manage_toolchain() {

    CHOICE=$(
        whiptail --title "Radio list example" --radiolist  \
            --fb --notags \
            "Escolha a versão do toolchain conforme o OS instalado" 0 0 0 \
            "64" "Versão 64 bits" ON \
            "32" "Versão 32 bits" OFF  3>&2 2>&1 1>&3

        )
    exitstatus=$?

    if [ $exitstatus -eq 0 ]; then

        if  [ $CHOICE -eq 32 ]; then
            url_file=$url_espressif_toolchain32
        else
            url_file=$url_espressif_toolchain64
        fi

        select_path "Seleção destino toolchain" "$toolchain_path_dest"
        exitstatus=$?
                
        if [ $exitstatus -eq 0 ]; then

            toolchain_path_dest=$filepath

            download_file "$url_file" "$HOME/Downloads/tc"

            #cria diretorio onde que foi escolhido como destino do toolchian
            mkdir -p $toolchain_path_dest
        
            #descompacta
            cd $toolchain_path_dest
            tar -xzf "$HOME/Downloads/tc"

            # procura onde estão os binarios 
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

            show_atencao "Instalação concluida.\nFazer logoff do usuario para atualizar as variáveis de ambiente"
        fi
    fi
}

function update_idf_repositorio() {

    cd $idf_path_dest

    is_full=$(ls | wc -l) > /dev/null

    if [ $is_full = 0 ]; then

        show_info "Clonagem iniciada"
        git clone --recursive --progress $idf_path_orin

    else
        UPSTREAM=${1:-'@{u}'}
        LOCAL=$(git rev-parse @)
        REMOTE=$(git rev-parse "$UPSTREAM")
        BASE=$(git merge-base @ "$UPSTREAM")

        if [ $LOCAL = $REMOTE ]; then
            show_info "Repositorio:\nAtualizado !\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
        elif [ $LOCAL = $BASE ]; then
            show_info "Repositorio:\nAtaulização iniciada\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
            git pull &
            pid=$?
            wait $pid

        elif [ $REMOTE = $BASE ]; then
            show_info "Repositorio:\nAtaulização iniciada\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
            git pull &
            pid=$?
            wait $pid
        else
            show_info "Repositorio:\nDiverge\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
        fi
    fi
}

function manage_idf() {

    if [ -n $IDF_PATH ]; then

        idf_path_dest="$IDF_PATH"
    fi

    while : ; do

        select_path "Repositório IDF" "$idf_path_dest"
        exitstatus=$?
            
        if [ $exitstatus = 0 ]; then

            idf_path_dest=$filepath

            if [[ -d "$idf_path_dest" || $(mkdir $idf_path_dest) = 0 ]]; then

                update_idf_repositorio
                break
            fi

            # TODO add/update no ~/.profile
            export IDF_PATH=$idf_path_dest

            show_info "Fazer logoff para atualizar as variáveis do ambiente."
        else
            break
        fi
    done
}

function install_dependencias() {

    sudo apt-get -y install git wget make libncurses-dev flex bison gperf python python-serial minicom
}

debug_screen() {

    CHOICE=$(
            whiptail --title "Debug" \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                --fb --notags\
                1 "Executar debug (prompt)"   \
                2 "Executar server(openocd)"  \
                3 "Interface (adaptador)"  \
                3 "Target (device)"  \
                5 "Monitor"  \
                6 "Scan server"   3>&2 2>&1 1>&3
            )
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        case $CHOICE in
                1) start_section_debug ;;
                2) start_debug_server ;;
                3) setup_interface ;;
                4) setup_target ;;
                5) esp32_monitor ;;
                6) scan_host ;;
        esac
    fi

    return $exitstatus
}

project_screen() {

    CHOICE=$(
            whiptail --title "Projeto" \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                --fb --notags\
                1 "Compilar todo o projeto"   \
                2 "Compilar app"  \
                3 "Criar projeto" 3>&2 2>&1 1>&3
            )
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        case $CHOICE in
                1) build_all ;;
                2) build_app ;;
                3) create_project ;;
        esac
    fi

    return $exitstatus
}

enviroment_screen() {

    CHOICE=$(
            whiptail --title "Ambiente de desenvolvimento" \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                --fb --notags\
                1 "Instalar/atualizar ESP-IDF"   \
                2 "Instalar/atualizar openocd"  \
                3 "Instalar/atualizar toolchain" \
                4 "Instala dependencias (pacotes)"  3>&2 2>&1 1>&3
            )
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        case $CHOICE in
                1) manage_idf ;;
                2) manage_openocd ;;
                3) manage_toolchain ;;
                4) install_dependencias ;;
        esac
    fi

   return $exitstatus
}

esp32_screen() {

    CHOICE=$(
            whiptail --title "ESP32" \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                --fb  --notags\
                1 "Gravar (uart)"   \
                2 "Monitor (uart)"  \
                3 "Configuração"  3>&2 2>&1 1>&3
            )
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        case $CHOICE in
                1) esp32_flash ;;
                2) esp32_monitor ;;
                3) esp32_config_screen ;;
        esac
    fi
   
   return $exitstatus
}

function main_screen() {

    CHOICE=$(
            whiptail --title "Principal" \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                --fb --notags\
                1 "Debug"   \
                2 "Projeto" \
                3 "ESP32" \
                4 "Ambiente de desenvolvimento" 3>&2 2>&1 1>&3
            )
    exitstatus=$?

    if [ $exitstatus = 0 ]; then

        case $CHOICE in
                1) show debug_screen ;;
                2) show project_screen ;;
                3) show esp32_screen ;;
                4) show enviroment_screen ;;
        esac
    else
        exit
    fi

    return $exitstatus
}

function show() {
    # mostra uma tela

    activate_screen=$1
}

function init_script() {
    # startup do script

    clear

    #permite acesso na porta serial
    sudo chmod 777 /dev/ttyUSB0 > /dev/null 
}

function main() {
    #Entrada do script

    init_script

    show main_screen

    # super loop no menu ativo
    while : ; do 

        $activate_screen; 
        exitstatus=$?

        # processa eventos com hanldes comunus
        if [ $exitstatus -ne 0 ]; then
            show main_screen
        fi
    done

    exit
}

main