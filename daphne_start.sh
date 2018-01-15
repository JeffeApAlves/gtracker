#!/bin/bash

NAME="Daphne"                                        #Name of the application (*)
DJANGODIR=/var/www/gtracker/web                      # Django project directory (*)
SOCKFILE=/var/www/gtracker/run/daphne.sock           # we will communicate using this unix socket (*)
#USER=gtracker                                       # the user to run as (*)
#USER_ID=999
#GROUP=webapps                                       # the group to run as (*)
#GROUP_ID=996
#NUM_WORKERS=5                                       # how many worker processes should Gunicorn spawn (*)
DJANGO_SETTINGS_MODULE=GoodsTracker.settings         # which settings file should Django use (*)
DJANGO_ASGI_MODULE=GoodsTracker.asgi                 # ASGI module name (*)
#ADDRESS=gtracker.com:8010
#PID_FILE=/tmp/gtracker_master.pid
ENVIROMENT=/home/jefferson/.virtualenvs/gtracker

echo "Starting $NAME as `whoami`"

export DJANGO_SETTINGS_MODULE=$DJANGO_SETTINGS_MODULE

cd $DJANGODIR
source $ENVIROMENT/bin/activate
daphne -u $SOCKFILE $DJANGO_ASGI_MODULE:channel_layer 




