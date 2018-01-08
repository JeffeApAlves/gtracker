#! /bin/bash
#
#  Gerenciador do projeto
#
#  <comando>    config: menu de configuração
#               update: atualiza
#
#
#  Steps:
#
#   1  Configurar a variavel de ambiente no  PROJECT_HOME=<path da solução> em gtracker.sh
#   
#   2. Incluir no .bashrc o script principal <gtracker.sh>
#
#   1. Conectar ambos (raspibarry+computador) em uma mesma rede com acesso a internet
#
#   3. Criar o mesmo usuario em ambos(raspiberry+computador)
#    3.1 adduser -m nome_usuario
#
#   4. Providenciar, para o usuaro criado sudo e bypass de senha para sudo atraves
#    4.1 visudo
#    4.2 <usuario> ALL=NOPASSWD: ALL
#
#   5. Providenciar RSA do seu usario
#    5.1 ssh-keygen -t rsa
#    5.2 ssh-copy-id user@ip_machine
#


BASEDIR="${0%/*}"
CALLDIR="${PWD}"

source $BASEDIR/def.sh


function update_enviroment(){

    # Atualiza os pacotes do ambiente Python

    # arquivo  temporario com a lista  dos pacotes python 
    local packages=/tmp/env_packages.txt

    pip freeze --local > $packages && pip install -U -r $packages

    pip freeze --local > "$PROJECT_HOME/web/requirements.txt"    
}

function install_enviroment(){

    # Instal os pacotes do ambiente Python

    pip install -r "$PROJECT_HOME/web/requirements.txt"    
}

function solution_deploy(){

    # Copia os arquivos para o servidor

    rsync -avz $PROJECT_HOME/tools $USER@$DEPLOY_GTRACKER
    rsync -avz $PROJECT_HOME/web $USER@$DEPLOY_GTRACKER
}

function run(){

    # Executa a aplicação django

    python $PROJECT_HOME/web/manage.py runserver  $IP_WEBSERVER:$PORT_WEBSERVER
}

function runworker(){

    # executa a aplicação django em modo produção

    python $PROJECT_HOME/web/manage.py runworker
}


##### Entrada do script

# Parametro 1 de entrada = comando
cmd=$1

if  [ $cmd = "update" ]; then

	update_enviroment

elif  [ $cmd = "deploy" ]; then

	solution_deploy

elif [ $cmd = "install" ]; then

	install_enviroment

elif [ $cmd = "run" ]; then

	run

elif [ $cmd = "runworker" ]; then

	runworker

else

    echo "Opção $cmd invalida. Comandos disponivies: update | install | deploy | run | runworker"
    exit -2
fi
