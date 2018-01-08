#! /bin/bash

# Diretorio onde se encontra o script
BASEDIR="${0%/*}"

project_path=$1
idf_path="$HOME/esp-idf"
idf_path_orin="https://github.com/espressif/esp-idf.git"
toolchain_path="$HOME/esp"
url_espressif_toolchain64="https://dl.espressif.com/dl/xtensa-esp32-elf-linux64-1.22.0-75-gbaf03c2-5.2.0.tar.gz"
url_espressif_toolchain32="https://dl.espressif.com/dl/xtensa-esp32-elf-linux32-1.22.0-75-gbaf03c2-5.2.0.tar.gz"
freq_jtag=10000
app_title="Gerenciador de projetos ESP32-Projeto:$project_name em $project_path"
template_project="$idf_path/examples/get-started/blink ."
makefile="$project_path/Makefile"
project_name=$(grep -m 1 PROJECT_NAME $makefile | sed 's/^.*= //g')
build_path="$project_path/build"
espfile="$project_path/esp_dbg.sh"
ext_program='elf'
ext_config='cfg'
serial=/dev/ttyUSB0

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

function update_paths() {

    file_temp=/tmp/profile

    sed -e '/IDF_PATH/D' $PROFILE_FILE | sed -e '/xtensa-esp32-elf/D' | cat > "$file_temp"

    echo  'export PATH=$PATH:'$toolchain_path >> "$file_temp"
    echo  'export IDF_PATH='$idf_path >> "$file_temp"

    export PATH="$PATH:$toolchain_path"
    export IDF_PATH=$idf_path

    cp "$file_temp" "$PROFILE_FILE"
}
