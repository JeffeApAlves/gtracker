#! /bin/bash
#
#  Gerenciador do projeto
#
#  Steps:
#
#   1  Configurar a variavel de ambiente PROJECT_NAME e PROJECT_HOME para o respectivo projeto
#   
#   2. Incluir no .bashrc esse script.
#       source <path_tools>/script_manage.sh
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

# Projeto
#
# nome do projeto
export PROJECT_NAME=GTRACKER
# Local do projeto
export PROJECT_HOME=$WORK_SPACE/gtracker

# System
#
PROFILE_FILE=$HOME/.bashrc

# Ambiente
#
PYTHON_VERSION=python3.6

# Local para deploy
#
export DEPLOYDIR=gtracker.com:/var/www/gtracker

# WEBSERVER
#
export PORT_WEBSERVER=8010
export IP_WEBSERVER=192.168.42.1

# Simulação
#
export SUMO_NAME=sp

# Coordenadas que define a região do mapa para simulação
export SUMO_BBOX='-23.6469,-46.7429,-23.6371,-46.7260'

export SUMO_OUTPUT=$PROJECT_HOME/vanet
export SUMO_TYPES='bus passenger truck pedestrian motorcycle'
export SUMO_SIMULATION=$SUMO_OUTPUT/$SUMO_NAME/$SUMO_NAME.sumocfg

# Classes/tipos que serão criadas
export SUMO_TYPES='bus passenger truck pedestrian motorcycle'

### Informações para geração da frota/tráfico ###

# Tempo de criação de viagens [segundos]
export SUMO_ENDTIME='{"passenger" : "3600", "bus" : "3600", "truck" : "3600", "pedestrian" : "3600", "bicycle" : "3600", "motorcycle" : "3600"}'

# Periodo do ciclo de criação das trips (ex: a cada x criar n trips)
export SUMO_PERIOD='{"passenger" : "3", "bus" : "10", "truck" : "7", "pedestrian" : "1", "bicycle" : "100", "motorcycle" : "4"}'

# Influencia na estatistica onde será iniciado as viagens
vSUMO_FRINGEFACTOR='{"passenger" : "5", "bus" : "5", "truck" : "5", "pedestrian" : "1", "bicycle" : "2", "motorcycle" : "3"}'

# Seed referencia da simulação
export SUMO_SEED=42

# ESP32
#
ESP32_NAME=""

#### Alias para os comandos dos scripts


# comandos para a aplicação
#
prj_name_lower=$(echo ${PROJECT_NAME,,})
alias script_manage='$PROJECT_HOME/tools/script_manage.py'
alias $prj_name_lower-env='workon script_manage'
alias $prj_name_lower-home='cd $PROJECT_HOME'
alias $prj_name_lower-deploy='script_manage deploy'
alias $prj_name_lower-update='script_manage update'
alias $prj_name_lower-install='script_manage install'
alias $prj_name_lower-run='script_manage run'
alias $prj_name_lower-worker='script_manage runworker'

# comandos sumo
#
alias sumo-run='script_manage sumo --cfg $SUMO_SIMULATION'
alias sumo-create='script_manage sumo --name "$SUMO_NAME" --seed "$SEED" --bbox "$SUMO_BBOX" --types "$SUMO_TYPES" --out "$SUMO_OUTPUT"'

# Configuração do ambiente de desenvolvimento para esp32
#
alias esp32-create='script_manage esp32'