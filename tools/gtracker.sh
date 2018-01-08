#! /bin/bash

#
# Local do projeto
#
export GTRACKER_HOME=$1

BASEDIR="$GTRACKER_HOME/tools"

source $BASEDIR/def.sh

#
# comandos gtracker
#
alias gtracker-h='cd $GTRACKER_HOME'
alias gtracker-d='$BASEDIR/prj_manage.sh deploy'
alias gtracker-u='$BASEDIR/prj_manage.sh update'
alias gtracker-i='$BASEDIR/prj_manage.sh install'
alias gtracker-r='$BASEDIR/prj_manage.sh run'
alias gtracker-w='$BASEDIR/prj_manage.sh runworker'


#
# Configuração do ambiente de desenvolvimento para esp32
#
alias esp32-c='$BASEDIR/config.sh'

#
# comandos Daphne
#
#alias daphne-r='daphne -b 127.0.0.1 -p 8020 --ws-protocol "graphql-ws" --proxy-headers GoodsTracker.asgi:channel_layer'
alias daphne-r='daphne -b 127.0.0.1 -p 8020 GoodsTracker.asgi:channel_layer --access-log=/var/www/gtracker/logs/daphne.log'


#
# comandos postgresql
#
alias pg-adm='python $WORKON_HOME/gtracker/lib/$PYTHON_VERSION/site-packages/pgadmin4/pgAdmin4.py'

#
# comandos ntop
#
alias ntop='sudo ntopng'

#
# comandos sumo
#
alias sumo-r='sumo-gui -c $SUMO_SIMULATION'
alias sumo-c='$BASEDIR/osmCreate.sh "$SUMO_NAME" -s "$SEED" -b "$SUMO_BBOX" -d "$SUMO_OUTPUT" -t "$SUMO_TYPES"'
alias sumo-i='$BASEDIR/osmCreate.sh "$SUMO_NAME" -i'
