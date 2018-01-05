#! /bin/bash
#
#  Gerenciador do projeto Goodstracker
#
#  Existe um segundo script "openocd.sh" que será rodado no server openocd (raspberry)
#  <comando>       config: menu de configuração
#                  update: atualiza
#                  deploy: sincroniza\atualiza arquivos  no webserver
# [opções]
#           -i <interface> Nome do arquivo correpsonente a interafce que sera usado no debug
#           -t <target> Nome do arquivo correpsonde ao target que sera debugado
#           -u <url> Url do repositorio do openocd para instala;
#
#  Menu para configuração de projetos com ESP32 utilizando o SDK IDF e OpenOCD para debug
#
#  Steps:
#
#   1. Conectar ambos (raspibarry+computador) em uma mesma rede com acesso a internet
#
#   2. Criar o mesmo usuario em ambos(raspiberry+computador)
#    2.1 adduser -m nome_usuario
#
#   3. Providenciar, para o usuaro criado sudo e bypass de senha para sudo atraves
#    3.1 visudo
#    3.2 <usuario> ALL=NOPASSWD: ALL
#
#   4. Providenciar RSA do seu usario
#    4.1 ssh-keygen -t rsa
#    4.2 ssh-copy-id user@ip_machine
#


#Ambiente
export IP=$(ifconfig | grep 'inet ' | awk '/192.168.42/{print $2}')

#usuario
user=$(whoami)

#Projeto Goodstrackers
BASE_PATH=/media/jefferson/Dados/workspace/WebAPP
export DEPLOY_GTRACKER=gtracker.com:/var/www/gtracker

#arquivo  temporario com a lista  dos pacotes python 
ENV_PACKAGES=/tmp/env_packages.txt

project_path=$(pwd)
url_openocd="https://github.com/espressif/openocd-esp32.git"
idf_path_orin="https://github.com/espressif/esp-idf.git"
toolchain_path="$HOME/esp"
idf_path="$HOME/esp-idf"
interface_path="/usr/local/share/openocd/scripts/interface"
target_path="/usr/local/share/openocd/scripts/target"
url_espressif_toolchain64="https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-61-gab8375a-5.2.0.tar.gz"
url_espressif_toolchain32="https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-61-gab8375a-5.2.0.tar.gz"
makefile="$project_path/Makefile"
gdbinit="$project_path/gdbinit"
project_name=$(grep -m 1 PROJECT_NAME $makefile | sed 's/^.*= //g')
target="$target_path/esp32.cfg" 
interface="$interface_path/raspberrypi-native.cfg"
program="$project_path/build/$project_name.elf"
host_gdb=$(grep -m 1 target $gdbinit | sed 's/^.*remote \|:.*/''/g')
build_path="$project_path/build"
openocdfile="$project_path/openocd.sh"
espfile="$project_path/esp_dbg.sh"
profile="$HOME/.bash_profile"
ext_program='elf'
ext_config='cfg'
before_screen=null
activate_screen=null
template_project="$idf_path/examples/get-started/blink ."
app_title="Gerenciador de projetos ESP32-Projeto:$project_name em $project_path"
freq_jtag=10000
serial=/dev/ttyUSB0

#Parametros de entrada
cmd=$1

function select_file() {

    local title=$1
    local path=$2
    local ext_file=$3
    local source=${4:-"LOCAL"}
    local dir_content=""
    local curdir=""

    if [ $source = "REMOTE" ]; then

        dir_content=$(ssh $user@$host_gdb "if [ ! -z $path ] ;then  cd $path ; fi ; ls -lhd */  *.$ext_file" 2>&1 | awk -F ' ' ' { print $9 " " $5 } ')
        curdir=$(ssh $user@$host_gdb "pwd" 2>&1)

    else

        if [ ! -z $path ] ; then
            cd "$path"
        fi

        dir_content=$(ls -lhd */  *.$ext_file | awk -F ' ' ' { print $9 " " $5 } ')
        curdir=$(pwd)
    fi

    if [ "$curdir" != "/" ] ; then
        dir_content="../ Voltar $dir_content"
    fi

    selection=$(dialog --stdout \
                    --title "Seleção de arquivo" \
                    --backtitle "$app_title" \
                    --scrollbar \
                    --menu "$title\nSelecione um arquivo do tipo '$ext_file'.\n$curdir" 30 100 20 \
                    $dir_content
                )
    local RET=$?

    if [ $RET -eq 0 ]; then

        if [[ -d "$selection" ]]; then

            select_file "$title" "$selection" "$ext_file" "$source"

        elif [[ -f "$selection" ]]; then

            if [[ $selection == *$ext_file ]]; then # verifica a exxtensão

                if (! dialog --title "Confirmação da seleção" --yesno "Diretório: $curdir\nArquivo  : $selection" 10 100 \
                            --yes-button "OK" \
                            --no-button "Voltar"); then

                    filename="$selection"
                    filepath="$curdir"
                    RET=0
                else
                    select_file "$title" "$curdir" "$ext_file" "$source"
                fi
            else
                show_msgbox "ERRO!" "Arquivo incompativel.\n$selection\nVoce deve selecionar um arquivo do tipo $ext_file"
                select_file "$title" "$curdir" "$ext_file" "$source"
            fi

        else # Não foi possivel ler o arquivo
            show_msgbox "ERRO!" "ERRO!" "Caminho ou arquivo invalido.\nNão foi possivel acessa-lo:$selection"
            select_file "$title" "$curdir" "$ext_file" "$source"
        fi
    fi

    return $RET
}

function select_path() {

    local title=$1
    local path=$2
    local source=${3:-"LOCAL"}
    local content_dir=""
    local cur_dir=""

    if [ $source = "REMOTE" ]; then

        content_dir=$(ssh $user@$host_gdb "if [ ! -z $path ] ;then  cd $path ; fi ; ls -lhd */" 2>&1 | awk -F ' ' ' { print $9 " " $5 } ')
        cur_dir=$(ssh $user@$host_gdb "pwd" 2>&1)

    else

        if [ ! -z $path ] ; then
            cd "$path"
        fi

        content_dir=$(ls -lhd */ | awk -F ' ' ' { print $9 " " $5 } ')
        cur_dir=$(pwd)
    fi

    if [ "$cur_dir" != "/" ] ; then
        content_dir="../ Voltar $content_dir" 
    fi


    selection=$(dialog --stdout \
                    --title "Seleção de diretório" \
                    --backtitle "$app_title" \
                    --extra-button --scrollbar \
                    --extra-label "Selecionar" \
                    --menu "Selecione o diretório de destino\n$cur_dir" 30 100 20 \
                    $content_dir
                )

    local RET=$?
 
    if [ $RET -eq 0 ]; then

        select_path "$title" "$selection"

    elif [ $RET -eq 3 ]; then #extra button=seleciona
 
        filepath="$cur_dir"
        RET=0
    fi

    return $RET
}

function show_msgbox() {

    dialog \
        --title "$1" \
        --backtitle "$app_title" \
        --msgbox "$2" \
        0 0
}

function show_info() {

    dialog \
        --title "Informações !" \
        --backtitle "$app_title" \
        --sleep 3 \
        --infobox "$1" \
        0 0
}

function show_description_file() {

    dialog \
        --title "Informações do arquivo" \
        --backtitle "$app_title" \
        --msgbox "Arquivo selecionado\nNome     : $1\nDiretorio: $2" \
        0 0
}

function show_description_sbc() {

    dialog \
        --title "Informações do SBC" \
        --backtitle "$app_title" \
        --msgbox "SBC encontrada\nIP: $1 " \
        0 0
}

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

    ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "start"  -i "$interface" -t "$target" &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$user iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function stop_debug_server() {

    ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "stop"  &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$user iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function start_section_debug() {

    select_program
    local RET=$?

    if [ $RET -eq 0 ]; then

        start_gdb "$gdbinit" "$program"
    fi
}

function setup_interface() {

    select_interface
}

function setup_target() {

    select_target
}

function create_project() {

    local new_project_name=""

    new_project_name=$(dialog --stdout \
                        --title "Nome do projeto" \
                        --backtitle "$app_title" \
                        --inputbox "Informe o nome do projeto" 10 100 project_name
                      )
    local RET=$?

    if [ $RET -ne 1 ]; then

        select_path "Seleção destino projeto" "$HOME"
        RET=$?

        if [ $RET -eq 0 ]; then

            cd $filepath
            cp -r $template_project
            mv "blink" $new_project_name

            cp "$espfile" "$filepath/$new_project_name"
            cp "$openocdfile" "$filepath/$new_project_name"


            sed -e '/PROJECT_NAME/D' "$filepath/$new_project_name/Makefile" > "/tmp/Makefile"
            echo  'PROJECT_NAME := '$new_project_name >> "/tmp/Makefile"
            cp "/tmp/Makefile" "$filepath/$new_project_name"
        fi
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

        ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "config" -i "$interface" -f "$freq_jtag" &> /dev/null
    
#        dialog \
#            --no-shadow \
#            --title "$user iniciando debug no host IP:$host_gdb" \
#            --tailbox /tmp/ssh.log 40 100
    
    fi
}

function build_all() {

    cd $project_path

    make clean &> /dev/null
    make all &> /tmp/build.log &

    dialog --title "Compilando projeto" --tailbox /tmp/build.log 30 100
}

function build_app() {

    cd $project_path

    make app &> /tmp/appbuild.log &

    dialog --title "Compilando app" --tailbox /tmp/appbuild.log 30 100
}

function esp32_flash() {

    cd $project_path

    if [[ -f "$serial" ]] ;then

        sudo chmod 777 $serial > /dev/null
    fi

    make flash &> /tmp/flash.log &

    dialog --title "Gravando ESP32" --tailbox /tmp/flash.log 30 100
}

function esp32_monitor() {

    cd $project_path

    if [[ -f "$serial" ]] ;then

        sudo chmod 777 $serial > /dev/null
    fi

    make monitor &> /tmp/monitor.log &

    dialog --title "Monitor (UART)" --tailbox /tmp/monitor.log 40 100
}

function esp32_config_screen() {

    cd $project_path

    make menuconfig
}

function scan_host() {

    host_gdb=$(sudo nmap -sn 192.168.0.0/24 | awk '/^Nmap/{ip=$NF}/B8:27:EB/{print ip}')

    show_description_sbc "$host_gdb"
}

function install_openocd() {

    ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "install" -u $url_openocd &> /tmp/ssh.log &

    dialog \
        --no-shadow \
        --title "$user iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function shutdown_interface() {

    ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "shutdown" &> /tmp/ssh.log  &

    dialog \
        --no-shadow \
        --title "$user iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function reset_target() {

    ssh $user@$host_gdb 'sudo -Sv && bash -s' -- < $openocdfile "reset" &> /tmp/ssh.log  &

    dialog \
        --no-shadow \
        --title "$user iniciando debug no host IP:$host_gdb" \
        --tailbox /tmp/ssh.log 40 100
}

function download_file() {

    local origem=$1
    local destino=$2

    wget "$origem" -O "$destino" 2>&1 | \
    stdbuf -o0 awk '/[.] +[0-9][0-9]?[0-9]?%/ { print substr($0,63,3) }' | \
    dialog --title "Download" --gauge "Por favor espere. Download em andamento.\n\nDe  :$origem\nPara:$destino" 0 0 0
}

function update_paths() {

    file_temp=/tmp/profile

    sed -e '/IDF_PATH/D' $profile | sed -e '/xtensa-esp32-elf/D' | cat > "$file_temp"

    echo  'export PATH=$PATH:'$toolchain_path >> "$file_temp"
    echo  'export IDF_PATH='$idf_path >> "$file_temp"

    export PATH="$PATH:$toolchain_path"
    export IDF_PATH=$idf_path

    cp "$file_temp" "$profile"
}

function install_toolchain() {

    CHOICE=$(dialog  --stdout\
                --title "Versão do Toolchain" \
                --radiolist "Escolha a versão do toolchain conforme o OS instalado" 0 0 0 \
                "64" "Versão 64 bits" ON \
                "32" "Versão 32 bits" OFF
            )
    local RET=$?

    if [ $RET -eq 0 ]; then


        if  [ $CHOICE == "32" ]; then
            url_file=$url_espressif_toolchain32
        else
            url_file=$url_espressif_toolchain64
        fi

        toolchain_path_dest="$HOME/esp" #default
        select_path "Seleção destino toolchain" "$toolchain_path_dest"
        RET=$?

        if [ $RET -eq 0 ]; then

            toolchain_path_dest=$filepath

            download_file "$url_file" "$HOME/Downloads/tc" &&

            #cria diretorio onde que foi escolhido como destino do toolchian
            mkdir -p $toolchain_path_dest &&

            #descompacta
            cd $toolchain_path_dest &&

            tar -xzf "$HOME/Downloads/tc" &&

            # procura onde estão os binarios 
            toolchain_path=$(find $toolchain_path_dest -name 'xtensa*gcc' | sed 's|/[^/]*$||' ) &&

            update_paths

            RET=0
        fi
    fi

    return $RET
}

function manage_toolchain() {

    install_toolchain
    local RET=$?

    if [ $RET -eq 0 ]; then

        show_msgbox "Atenção" "Instalação concluida.\nFazer logoff do usuario para atualizar as variáveis de ambiente"
    fi
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

    local destino=$1
    local UPSTREAM=${2:-'@{u}'}   #[opcional] passar o branch  ex:release/v2.1"

    cd $destino &&
    git remote update > /dev/null &&
    #git status -uno  > /dev/null &&

    LOCAL=$(git rev-parse @) && 
    REMOTE=$(git rev-parse "$UPSTREAM") &&
    BASE=$(git merge-base @ "$UPSTREAM") &&

    if [ $LOCAL = $REMOTE ]; then
        show_info "Atualizado !\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    elif [ $LOCAL = $BASE ]; then

        git submodule update
        git pull &> /tmp/git.log &> /tmp/git.log 30 100 &
        dialog \
            --title "Atualização respositório-Local :$LOCAL\nRemote:$REMOTE\nBase  :$BASE" \
            --tailbox /tmp/git.log 30 100

    elif [ $REMOTE = $BASE ]; then

        git submodule update
        git pull &> /tmp/git.log &> /tmp/git.log 30 100 &
        dialog
            --title "Atualização respositório-Local :$LOCAL\nRemote:$REMOTE\nBase  :$BASE" \
            --tailbox /tmp/git.log 30 100
      else

        show_info "Divergencias\n\nLocal :$LOCAL\nRemote:$REMOTE\nBase  :$BASE"
    fi
}

function manage_idf() {
    # Atualiza ou clona o repositorio IDF

    if [ -n $IDF_PATH ]; then

        idf_path="$IDF_PATH"
    fi

    select_path "Repositório IDF" "$idf_path"
    local RET=$?

    if [ $RET -eq 0 ]; then

        idf_path=$filepath

        if [[ -d "$idf_path" || $(mkdir $idf_path) -eq 0 ]]; then

            {
                update_repositorio "$idf_path"

            } || {

                clone_repositorio "$idf_path_orin" "$idf_path"

            } || {

                show_msgbox "ERRO!" "Não foi possivel clonar/atualizar o SDK"
            }
        fi

        update_paths
    fi
}

function install_dependencias() {
    # Instala pacotes necessários

    sudo apt-get -y install \
        nmap \
        git \
        wget \
        make \
        dialog \
        libncurses5-dev \
        flex \
        bison \
        gperf \
        python \
        python-serial \
        minicom  &> /tmp/install.log &

    dialog --title "Instalação dos pacotes" --tailbox  /tmp/install.log 30 100
}

debug_screen() {
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
                4) setup_interface ;;
                5) setup_target ;;
                6) config_jtag ;;
                7) reset_target ;;
                8) scan_host ;;
                9) shutdown_interface ;;
        esac
    fi

    return $RET
}

project_screen() {
    # Tela gerenciamento do projeto

    CHOICE=$(dialog --stdout \
                --title "Projeto" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Compilar todo o projeto"   \
                2 "Compilar app"  \
                3 "Criar projeto"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) build_all ;;
                2) build_app ;;
                3) create_project ;;
        esac
    fi

    return $RET
}

enviroment_screen() {
    # Tela para configuração do ambiente de desenvolvimento

    CHOICE=$(dialog --stdout \
                --title "Ambiente de desenvolvimento" \
                --backtitle "$app_title" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 20 100 20 \
                1 "Instalar/atualizar ESP-IDF"   \
                2 "Instalar/atualizar openocd"  \
                3 "Instalar/atualizar toolchain" \
                4 "Instalar/atualizar dependencias (pacotes)"
            )
    local RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) manage_idf ;;
                2) install_openocd ;;
                3) manage_toolchain ;;
                4) install_dependencias ;;
        esac
    fi

    return $RET
}

esp32_screen() {
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
                1) esp32_flash ;;
                2) esp32_monitor ;;
                3) esp32_config_screen ;;
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

    before_screen=$activate_screen

    activate_screen=$1
}

function install_dialog() {

    echo "Por favor espere..."

    local pacote=$(dpkg --get-selections | grep "dialog" )

    if [ ! -n "$pacote" ] ; then

        sudo apt-get install -y  dialog -qq > /dev/null
    fi

    pacote=$(dpkg --get-selections | grep "libncurses5-dev" )

    if [ ! -n "$pacote" ] ; then

        sudo apt-get libncurses5-dev -y  dialog -qq > /dev/null
    fi
}

function init_script() {
    # startup do script

    #limpa a tela
    clear

    install_dialog

    #permite acesso na porta serial
    if [[ -f "$serial" ]] ;then

        sudo chmod 777 $serial > /dev/null
    fi

    #inicializa com o menu principal
    show_screen main_screen
}

function event_process() {
    # super loop no menu ativo

    while : ; do

        $activate_screen;
        RET=$?

        # processa eventos com hanldes comunus
        if [ $RET -ne 0 ]; then

            if [ $activate_screen = main_screen ]; then
                # ESC na tela principal entao sai do script
                break
            else
                show_screen $before_screen
            fi
        fi
    done
}

##### Main - Entrada do script

init_script


if [ $cmd = "config" ]; then

    event_process

    clear
elif  [ $cmd = "update" ]; then

    pip freeze --local > $ENV_PACKAGES && pip install -U -r $ENV_PACKAGES

elif  [ $cmd = "deploy" ]; then

    rsync -avz $GTRACKER_HOME $user@$DEPLOY_GTRACKER

else
  echo "Opção $cmd invalida. Comandos disponivies: config | update | deploy"
  exit -2
fi
