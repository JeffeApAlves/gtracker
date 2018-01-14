#! /bin/bash
#
#  Gerenciador do projeto
#
#  Steps:
#
#   1  Configurar a variavel de ambiente PROJECT_NAME e PROJECT_HOME para o respectivo projeto
#   
#   2. Incluir no .bashrc esse script.
#       source <path_tools>/<nome_do_script>.sh
#
#   3. Conectar o webserver e o computador utilizado para o desenvolvimento em uma mesma rede com acesso a internet
#
#   4. Criar o mesmo usuario em ambos
#    4.1 adduser -m nome_usuario
#
#   5. Providenciar, para o usuaro criado sudo e bypass de senha para sudo atraves
#    5.1 visudo
#    5.2 <usuario> ALL=NOPASSWD: ALL
#
#   6. Providenciar RSA do seu usario
#    6.1 ssh-keygen -t rsa
#    6.2 ssh-copy-id <use>r@<ip-machine>
#

#### Definições e parametros do projeto

# Projeto #

# nome do projeto
PROJECT_NAME=GTRACKER

# Local do projeto
PROJECT_HOME=$WORK_SPACE/gtracker

# nome do projeto
export PROJECT_CONF=$PROJECT_HOME/${PROJECT_NAME,,}.conf


#### Alias para os scripts

prj_name_lower=$(echo ${PROJECT_NAME,,})
alias app-manage=$PROJECT_HOME/tools/app-manage.py
alias vanet=$PROJECT_HOME/tools/vanet-manage.py
alias platform=$PROJECT_HOME/tools/platform-manage.py

# aplicação #

alias $prj_name_lower='app-manage'
alias $prj_name_lower-home='cd $PROJECT_HOME'
alias $prj_name_lower-env='workon $prj_name_lower'
alias $prj_name_lower-deploy='app-manage deploy'
alias $prj_name_lower-update='app-manage update'
alias $prj_name_lower-install='app-manage install'
alias $prj_name_lower-run='app-manage run'
alias $prj_name_lower-runworker='app-manage run --worker'

# sumo #

# executa a simulação com o modo gui habilitado
alias sumo-run='vanet run'
# cria a simulação
alias sumo-create='vanet create'

# platform (ESP32) #

# Configuração do ambiente de desenvolvimento para esp32
alias platform-create='platform create'


# Configuração do ambiente de desenvolvimento para esp32
alias platform-menu='platform config --menu'