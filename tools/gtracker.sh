#! /bin/bash
#
#  Gerenciador do projeto
#
#  Steps:
#
#   1  Configurar a variavel de ambiente no  PROJECT_NAME e PROJECT_HOME para o respectivo projeto
#   
#   2. Incluir no .bashrc esse script.
#       source $WORK_SPACE/gtracker/tools/gtracker.sh
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
#    6.2 ssh-copy-id user@ip_machine
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

#
# comandos gtracker
#
alias gtracker='$PROJECT_HOME/tools/gtracker.py'
alias gtracker-env='workon gtracker'
alias gtracker-home='cd $PROJECT_HOME'
alias gtracker-deploy='gtracker deploy'
alias gtracker-update='gtracker update'
alias gtracker-install='gtracker install'
alias gtracker-run='gtracker run'
alias gtracker-worker='gtracker runworker'

# comandos sumo
#
alias sumo-run='gtracker sumo --cfg $SUMO_SIMULATION'
alias sumo-create='gtracker sumo --name "$SUMO_NAME" --seed "$SEED" --bbox "$SUMO_BBOX" --types "$SUMO_TYPES" --out "$SUMO_OUTPUT"'

#
# Configuração do ambiente de desenvolvimento para esp32
#
alias esp32-create='gtracker esp32'

#
# comandos Daphne
#
#alias daphne-r='daphne -b 127.0.0.1 -p 8020 --ws-protocol "graphql-ws" --proxy-headers GoodsTracker.asgi:channel_layer'
alias daphne-r='daphne -b 127.0.0.1 -p 8020 GoodsTracker.asgi:channel_layer --access-log=/var/www/gtracker/logs/daphne.log'

#
# comandos postgresql
#
alias pg-adm='python $WORKON_HOME/gtracker/lib/$PYTHON_VERSION/site-packages/pgadmin4/pgAdmin4.py'