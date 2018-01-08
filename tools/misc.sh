#! /bin/bash
#
#
#

# Diretorio onde se encontra o script
BASEDIR="${0%/*}"

source $BASEDIR/def.sh

back_title="Projeto $PROJECT_NAME"


function select_file() {

    local title=$1
    local path=$2
    local ext_file=$3
    local source=${4:-"LOCAL"}
    local dir_content=""
    local curdir=""

    if [ $source = "REMOTE" ]; then

        dir_content=$(ssh $USER@$host_gdb "if [ ! -z $path ] ;then  cd $path ; fi ; ls -lhd */  *.$ext_file" 2>&1 | awk -F ' ' ' { print $9 " " $5 } ')
        curdir=$(ssh $USER@$host_gdb "pwd" 2>&1)

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
                    --backtitle "$back_title" \
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

        content_dir=$(ssh $USER@$host_gdb "if [ ! -z $path ] ;then  cd $path ; fi ; ls -lhd */" 2>&1 | awk -F ' ' ' { print $9 " " $5 } ')
        cur_dir=$(ssh $USER@$host_gdb "pwd" 2>&1)

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
                    --backtitle "$back_title" \
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
        --backtitle "$back_title" \
        --msgbox "$3" \
        0 0
}

function show_info() {

    dialog \
        --title $1 \
        --backtitle "$back_title" \
        --sleep 3 \
        --infobox "$3" \
        0 0
}


function show_description_file() {

    dialog \
        --title "Informações do arquivo" \
        --backtitle "$back_title" \
        --msgbox "Arquivo selecionado\nNome     : $1\nDiretorio: $2" \
        0 0
}

function show_description_sbc() {

    dialog \
        --title "Informações do SBC" \
        --backtitle "$back_title" \
        --msgbox "SBC encontrada\nIP: $1 " \
        0 0
}

function download_file() {

    local origem=$1
    local destino=$2

    wget "$origem" -O "$destino" 2>&1 | \
    stdbuf -o0 awk '/[.] +[0-9][0-9]?[0-9]?%/ { print substr($0,63,3) }' | \
    dialog --title "Download" --gauge "Por favor espere. Download em andamento.\n\nDe  :$origem\nPara:$destino" 0 0 0
}

#function clone_repositorio() {
#
#    # realiza de um repositorio no git
#
#    local origem=$1
#    local destino=$2
#
#    git clone --recursive $origem $destino
#}

function clone_repositorio() {

    local origem=$1
    local destino=$2

    git clone --recursive --progress $origem 2>&1 | \

    stdbuf -o0 awk 'BEGIN{RS="\r|\n|\r\n|\n\r";ORS="\n"}/Receiving/{print substr($3, 1, length($3)-1)}' | \
    dialog --title "Download" --gauge "Por favor espere. Clonagem em andamento.\n\nDe  :$origem\nPara:$destino" 0 0 0
}

function install_dependencias() {
    # Instala pacotes necessários

    sudo apt-get -y install \
        git \
        make \
        wget \
        nmap \
        flex \
        bison \
        gperf \
        python \
        python-serial \
        minicom  &> /tmp/install.log &

    dialog --title "Instalação dos pacotes" --tailbox  /tmp/install.log 30 100
}

#function install_dialog() {
#
#    echo "Por favor espere..."
#
#	for i in ${TYPES[@]}; do
#
#        local pacote=$(dpkg --get-selections | grep "${i}" )
#
#
#        if [ ! -n "$pacote" ] ; then
#
#            sudo apt-get install -y  dialog -qq > /dev/null
#        fi		
#
#	done
#
#
#    local pacote=$(dpkg --get-selections | grep "dialog" )
#
#    if [ ! -n "$pacote" ] ; then
#
#        sudo apt-get install -y  dialog -qq > /dev/null
#    fi
#
#    pacote=$(dpkg --get-selections | grep "libncurses5-dev" )
#
#    if [ ! -n "$pacote" ] ; then
#
#        sudo apt-get libncurses5-dev -y  dialog -qq > /dev/null
#    fi
#}
