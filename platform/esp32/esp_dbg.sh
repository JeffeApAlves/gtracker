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
interfacefile=""
targetfile=""
programfile="$project_path/build/$project_name.elf"
host_gdb=$(grep -m 1 target $gdbinitfile | sed 's/^.*remote \|:.*/''/g')
build_path="$project_path/build"
openocdfile="$project_path/openocd.sh"
espfile="$project_path/esp_dbg.sh"
ext_program='elf'
ext_config='cfg'
activate_screen=0
template_project="$IDF_PATH/examples/get-started/blink ."

function select_file() {

    local title=$1
    local path=$2
    local ext_file=$3

    if [ ! -z $2 ] ; then
        cd "$path"
    fi

    # conteudo do diretorio
    dir_content=$(ls -lhd */  *.$ext_file | awk -F ' ' ' { print $9 " " $5 } ')
    curdir=$(pwd)

    if [ "$curdir" != "/" ] ; then
        dir_content="../ Voltar $dir_content" 
    fi

    selection=$(dialog --stdout \
                    --title "Seleção de arquivo" \
                    --scrollbar \
                    --menu "Selecione um arquivo com do tipo '$ext_file'.\n$curdir" 30 100 20 \
                    $dir_content
                )
    RET=$?

    if [ $RET -eq 0 ]; then

        if [[ -d "$selection" ]]; then 
            
            select_file "$title" "$selection" "$ext_file"
        
        elif [[ -f "$selection" ]]; then 
 
            if [[ $selection == *$ext_file ]]; then # verifica a exxtensão 
                
                if (dialog --title "Confirmação da seleção" --yesno "Diretório: $curdir\nArquivo  : $selection" 10 100 \
                            --yes-button "OK" \
                            --no-button "Voltar"); then
                    
                    filename="$selection"
                    filepath="$curdir"
                    RET=0
                else
                    select_file "$title" "$curdir" "$ext_file"
                fi
            else
                show_erro "Arquivo incompativel" "$selection\nVoce deve selecionar um arquivo do tipo $ext_file"
                select_file "$title" "$curdir" "$ext_file"
            fi
 
        else # Não foi possivel ler o arquivo
            show_erro "Caminho ou arquivo invalido" "Não foi possivel acessa-lo:$selection"
            select_file "$title" "$curdir" "$ext_file"
        fi
    fi

    return $RET
}

function select_path() {

    local title=$1
    local path=$2

    if [ ! -z $2 ] ; then
        cd "$path"
    fi

    content_dir=$(ls -lhd */ | awk -F ' ' ' { print $9 " " $5 } ')
    cur_dir=$(pwd)

    if [ "$cur_dir" != "/" ] ; then
        content_dir="../ Voltar $content_dir" 
    fi

    selection=$(dialog --stdout \
                    --title "Seleção de diretório" \
                    --extra-button --scrollbar \
                    --extra-label "Selecionar" \
                    --menu "Selecione o diretório de destino\n$cur_dir" 30 100 20 \
                    $content_dir
                )
    RET=$?
 
    if [ $RET -eq 0 ]; then

        select_path "$title" "$selection"

    elif [ $RET -eq 3 ]; then #extra button
 
        filepath="$cur_dir"
        RET=0
    fi

    return $RET
}

function show_erro() {

    dialog --title "ERROR:$1 " --msgbox "$2" 0 0
}

function show_info() {

    dialog --title "Informações" --infobox "$1" 0 0
    sleep 3
    clear
}

function show_atencao() {

    dialog \ 
        --title "ATENÇÂO" \
        --msgbox "$1" \
        0 0
}

function show_description_file() {

    dialog \ 
        --title "Informações do arquivo" \
        --msgbox "Arquivo selecionado\nNome     : $1\nDiretorio: $2" \
        0 0 
}

function show_description_sbc() {

    dialog \
        --title "Informações do SBC" \
        --msgbox "SBC encontrada\nIP: $1 " \
        0 0
}

function select_program() {

    select_file "Selecione o programa" "$build_path" "$ext_program"
    RET=$?

    if [ $RET -eq 0 ]; then

        programfile=$filepath/$filename
    fi

    return $RET
}

function select_interface(){

    select_file "Selecione o adaptador (interface)" "$interface_path" "$ext_config"
    RET=$?

    if [ $RET -eq 0 ]; then

        interfacefile=$filepath/$filename
    fi

    return $RET
}

function select_target() {

    select_file "Selecione o target" "$target_path" "$ext_config"
    RET=$?

    if [ $RET = 0 ]; then

        targetfile=$filepath/$filename
    fi

    return $RET
}

function start_gdb() {
    local init=$1
    local program=$2

    if [ -f $init ] ; then
        if [ -f $init ] ; then
            xtensa-esp32-elf-gdb -x $init $program
        else
            show_atencao "Não foi possivel localizar o arquivo de debug.\n$program"
        fi
    else
        show_atencao "Não foi possivel localizar o arquivo de inicializacao.\n$init"
    fi
}

function start_debug_server() {

    user=$(whoami)

    ssh $user@$host_gdb 'sudo -Sv && bash -s' < $openocdfile &> /tmp/ssh.log &

    dialog --title "$user iniciando debug no host IP:$host_gdb" --tailbox /tmp/ssh.log 30 100
}

function start_section_debug() {

    select_program
    RET=$?

    if [ $RET -eq 0 ]; then

        start_gdb "$gdbinitfile" "$programfile"
    fi
}

function setup_interface() {

    select_interface
}

function setup_target() {

    select_target
}

function create_project() {

    new_project_name=$(dialog 
                         --title "Nome do projeto" \
                         --inputbox "Informe o nome do projeto" 10 100 project_name
                       )
    RET=$?
                
    if [ $RET -eq 0 ]; then

        select_path "Seleção destino projeto" "$HOME"
        RET=$?
                    
        if [ $RET -eq 0 ]; then

            cd $filepath
            cp -r $template_project
            mv "blink" $new_project_name

            cp "$espfile" "$filepath/$new_project_name"
            cp "$openocdfile" "$filepath/$new_project_name"

            #TODO renomear propriedade arquivo
        fi
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

    make flash &> /tmp/flash.log &

    dialog --title "Gravando ESP32" --tailbox /tmp/flash.log 30 100
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

    show_description_sbc "$host_gdb"
}

function manage_openocd() {

    echo "TODO"
}

function download_file() {
    local origem=$1
    local destino=$2

    wget "$origem" -O "$destino" 2>&1 | \
    stdbuf -o0 awk '/[.] +[0-9][0-9]?[0-9]?%/ { print substr($0,63,3) }' | \
    dialog --title "Download" --gauge "Por favor espere. Download em andamento.\n\nDe  :$origem\nPara:$destino" 0 0 0
}

function install_toolchain() {

    CHOICE=$(dialog  --stdout\
                --title "Versão do Toolchain" \
                --radiolist "Escolha a versão do toolchain conforme o OS instalado" 0 0 0 \
                "64" "Versão 64 bits" ON \
                "32" "Versão 32 bits" OFF  
            )
    RET=$?

    if [ $RET -eq 0 ]; then

        if  [ $CHOICE == "32" ]; then
            url_file=$url_espressif_toolchain32
        else
            url_file=$url_espressif_toolchain64
        fi

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

            #TODO atualizacao profile
            #teste=$(grep -m 1 PATH "$HOME/.bash_profile")
            #echo $teste
            #if [ -z $teste ]; then

                # Atualizar profile 
                export PATH="$PATH:$toolchain_path"
                # TODO add/update no ~/.profile
                #echo "OK"
            #fi

            RET=0
        fi
    fi

    return $RET
}

function manage_toolchain() {

    install_toolchain
    RET=$?

    if [ $RET -eq 0 ]; then

        show_atencao "Instalação concluida.\nFazer logoff do usuario para atualizar as variáveis de ambiente"
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

function manage_idf() {

    if [ -n $IDF_PATH ]; then

        idf_path_dest="$IDF_PATH"
    fi

    select_path "Repositório IDF" "$idf_path_dest"
    RET=$?
        
    if [ $RET -eq 0 ]; then

        idf_path_dest=$filepath

        if [[ -d "$idf_path_dest" || $(mkdir $idf_path_dest) = 0 ]]; then

            { 
                update_repositorio "$idf_path_dest"
            } || { 
                
                clone_repositorio "$idf_path_orin" "$idf_path_dest" 
            } || {

                show_info "Não foi possivel clonar/atualizar o SDK"
            }
        fi

        # TODO add/update no ~/.profile
        export IDF_PATH=$idf_path_dest

        show_info "Fazer logoff para atualizar as variáveis do ambiente."
    fi
}

function install_dependencias() {

    sudo apt-get -y install git wget make libncurses-dev flex bison gperf python python-serial minicom  &> /tmp/install.log &

    dialog --title "Instalação dos pacotes" --tailbox  /tmp/install.log 30 100
}

debug_screen() {

    CHOICE=$(dialog --stdout\
                --title "Debug" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                1 "Executar debug (prompt)"   \
                2 "Executar server(openocd)"  \
                3 "Interface (adaptador)"  \
                3 "Target (device)"  \
                5 "Monitor"  \
                6 "Scan server"
            )
    RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                '1') start_section_debug ;;
                '2') start_debug_server ;;
                '3') setup_interface ;;
                '4') setup_target ;;
                '5') esp32_monitor ;;
                '6') scan_host ;;
        esac
    fi

    return $RET
}

project_screen() {

    CHOICE=$(dialog --stdout \
                --title "Projeto" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                1 "Compilar todo o projeto"   \
                2 "Compilar app"  \
                3 "Criar projeto"
            )
    RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                '1') build_all ;;
                '2') build_app ;;
                '3') create_project ;;
        esac
    fi

    return $RET
}

enviroment_screen() {

    CHOICE=$(dialog --stdout \
                --title "Ambiente de desenvolvimento" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                1 "Instalar/atualizar ESP-IDF"   \
                2 "Instalar/atualizar openocd"  \
                3 "Instalar/atualizar toolchain" \
                4 "Instalar dependencias (pacotes)"
            )
    RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                '1') manage_idf ;;
                '2') manage_openocd ;;
                '3') manage_toolchain ;;
                '4') install_dependencias ;;
        esac
    fi

    return $RET
}

esp32_screen() {

    CHOICE=$(dialog --stdout \
                --title "ESP32" \
                --no-tags \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                1 "Gravar (uart)"   \
                2 "Monitor (uart)"  \
                3 "Configuração"
            )
    RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                '1') esp32_flash ;;
                '2') esp32_monitor ;;
                '3') esp32_config_screen ;;
        esac
    fi
   
    return $RET
}

function main_screen() {

    CHOICE=$(dialog --stdout \
                --title "Principal"\
                --no-tags --scrollbar \
                --menu "Selecione uma das opções abaixo" 30 100 20 \
                1 "Debug" \
                2 "Projeto" \
                3 "ESP32" \
                4 "Ambiente de desenvolvimento" 
             )
    RET=$?

    if [ $RET -eq 0 ]; then

        case $CHOICE in
                1) show_screen debug_screen ;;
                2) show_screen project_screen ;;
                3) show_screen esp32_screen ;;
                4) show_screen enviroment_screen ;;
        esac
    else
        clear
        exit
    fi

    return $RET
}

function show_screen() {
    # selecioan um menu para ser mostrado na tela
   
    activate_screen=$1
}

function init_script() {
    # startup do script

    sudo apt-get install -y  dialog -qq

    #limpa a tela
    clear

    #permite acesso na porta serial
    sudo chmod 777 /dev/ttyUSB0 > /dev/null 

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
            show_screen main_screen
        fi
    done
}

#Entrada do script

init_script

event_process

clear