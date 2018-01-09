#! /bin/bash

#
# Projeto
#
PROJECT_NAME=GTRACKER
PROJECT_HOME=$GTRACKER_HOME

#
# System
#
PROFILE_FILE=$HOME/.bashrc

#
# Ambiente
#
PYTHON_VERSION=python3.6

#
# Local para deploy
#
export DEPLOYDIR=gtracker.com:/var/www/gtracker

#
# WEBSERVER
#
export PORT_WEBSERVER=8010
export IP_WEBSERVER=192.168.42.1

#
# Simulação
#
export SUMO_NAME=sp

# Coordenadas que define a região do mapa para simulação
SUMO_BBOX='-23.6469,-46.7429,-23.6371,-46.7260'

SUMO_OUTPUT=$PROJECT_HOME/vanet
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

#
# ESP32
#
ESP32_NAME=""