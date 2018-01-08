#! /bin/bash

#
# Projeto
#
PROJECT_NAME=GTRACKER
PROJECT_HOME=$GTRACKER_HOME

#
# System
#
PROFILE_FILE="$HOME/.bashrc"

#
# Ambiente
#
PYTHON_VERSION=python3.6

#
# Local para deploy
#
DEPLOY_GTRACKER=gtracker.com:/var/www/gtracker

#
# WEBSERVER
#
PORT_WEBSERVER=8010
IP_WEBSERVER=192.168.42.1

#
# Simulação
#
SUMO_NAME=sp
SUMO_BBOX="-23.6469,-46.7429,-23.6371,-46.7260"
SEED=42
SUMO_OUTPUT=$PROJECT_HOME/vanet
SUMO_TYPES="bus passenger truck pedestrian motorcycle"
SUMO_SIMULATION=$SUMO_OUTPUT/$SUMO_NAME/$SUMO_NAME.sumocfg