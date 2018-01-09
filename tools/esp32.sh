#! /bin/bash

# Diretorio onde se encontra o projeto ESP32
BASEDIR=$HOME

idf_path="$HOME/esp-idf"

# Repositorio git do SDK
idf_path_orin="https://github.com/espressif/esp-idf.git"

# Path selecionada para toolchain
toolchain_path="$HOME/esp"

# Template de um projeto em branco
template_project="$idf_path/examples/get-started/blink ."

makefile="$BASEDIR/Makefile"

PROJECT_NAME=$(grep -m 1 PROJECT_NAME $makefile | sed 's/^.*= //g')

OUTPUTDIR="$BASEDIR/build"

# Local de download dos toolchains na internet
url_espressif_toolchain64="https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-75-gbaf03c2-5.2.0.tar.gz"
url_espressif_toolchain32="https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-75-gbaf03c2-5.2.0.tar.gz"

# porta serial
SERIAL_PORT=/dev/ttyUSB0

function create_project() {

    local new_project_name=""

    new_project_name=$(dialog --stdout \
                        --title "Nome do projeto" \
                        --backtitle "Gerenciador de projetos ESP32-Projeto:$PROJECT_NAME em $BASEDIR" \
                        --inputbox "Informe o nome do projeto" 10 100 PROJECT_NAME
                      )
    local RET=$?

    if [ $RET -ne 1 ]; then

        select_path "Seleção destino projeto" "$HOME"
        RET=$?

        if [ $RET -eq 0 ]; then

            cd $filepath
            cp -r $template_project
            mv "blink" $new_project_name

            sed -e '/PROJECT_NAME/D' "$filepath/$new_project_name/Makefile" > "/tmp/Makefile"
            echo  'PROJECT_NAME := '$new_project_name >> "/tmp/Makefile"
            cp "/tmp/Makefile" "$filepath/$new_project_name"
        fi
    fi
}

function build_all() {

    cd $BASEDIR

    make clean &> /dev/null
    make all &> /tmp/build.log &

    dialog --title "Compilando projeto" --tailbox /tmp/build.log 30 100
}

function build_app() {

    cd $BASEDIR

    make app &> /tmp/appbuild.log &

    dialog --title "Compilando app" --tailbox /tmp/appbuild.log 30 100
}

function serial_permission(){

    # permite acesso na porta serial
    
    if [[ -f "$SERIAL_PORT" ]] ;then

        sudo chmod 777 $SERIAL_PORT > /dev/null
    fi
}

function esp32_flash() {

    cd $BASEDIR

    serial_permission

    make flash &> /tmp/flash.log &

    dialog --title "Gravando ESP32" --tailbox /tmp/flash.log 30 100
}

function esp32_monitor() {

    cd $BASEDIR

    serial_permission

    make monitor &> /tmp/monitor.log &

    dialog --title "Monitor (UART)" --tailbox /tmp/monitor.log 40 100
}

function esp32_config {

    cd $BASEDIR

    make menuconfig
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

function update_paths() {

 #   file_temp=/tmp/profile

 #   sed -e '/IDF_PATH/D' $PROFILE_FILE | sed -e '/xtensa-esp32-elf/D' | cat > "$file_temp"

 #   echo  'export PATH=$PATH:'$toolchain_path >> "$file_temp"
 #   echo  'export IDF_PATH='$idf_path >> "$file_temp"

 #   export PATH="$PATH:$toolchain_path"
 #   export IDF_PATH=$idf_path

 #   cp "$file_temp" "$PROFILE_FILE"
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

            show_msgbox "Atenção" "Instalação concluida.\nFazer logoff do usuario para atualizar as variáveis de ambiente"

            RET=0
        fi
    fi

    return $RET
}

##### Entrada do script

# Parametro 1 de entrada = comando
cmd=$1

# Pula o primeiro argumento (comando)
shift

while getopts "b:o:t:d:s:i" opt; do

  case $opt in

    o)  INPUT_OSM_FILE=$OPTARG
	    SOURCE_OSM=2
	    CMD=create
	;;

    t)  set_types $OPTARG
	;;

    d)  BASEDIR=$OPTARG
	;;

    s)  scope=$OPTARG
	;;

    i)  install_dependencias
	;;

    b)	BBOX=$OPTARG
		SOURCE_OSM=1
        CMD=create
	;;

    \?)
	echo "Opção invalida: -$OPTARG" >&2
      ;;

    :)
	echo "Opção -$OPTARG requer um argumento." >&2
	exit 1
      ;;

  esac

done

  
if  [ $cmd = "create" ]; then

    create_project

elif  [ $cmd = "install" ]; then

    if [ $scope = "toolchain" ]; then

	    install_toolchain

    elif [ $scope = "sdk" ]; then

        manage_idf

    else
        echo "opção inválida $scope"
    fi

if  [ $cmd = "update" ]; then

    if [ $scope = "toolchain" ]; then

	    install_toolchain

    elif [ $scope = "sdk" ]; then

        update_repositorio

    else
        echo "opção inválida $scope"
    fi

elif [ $cmd = "build" ]; then

    if [ $scope = "all" ]; then

	    build_all

    elif [ $scope = "app" ]; then

        build_app

    else
        echo "opção inválida $scope"
    fi

elif [ $cmd = "esp" ]; then

    if [ $scope = "flash" ]; then

	    esp32_flash

    elif [ $scope = "monitor" ]; then

        esp32_monitor

    elif [ $cmd = "config" ]; then

	    esp32_config

    else
        echo "opção inválida $scope"
    fi

else

    echo "Comando $cmd invalida. Comandos disponivies: install | update | create | build | esp"
    exit -2
fi
