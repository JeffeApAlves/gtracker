#!/bin/bash

NAME="gtracker-workers"                              # Name of the application (*)
DJANGODIR=/var/www/gtracker/web          
USER=gtracker                                        # the user to run as (*)
USER_ID=999
GROUP=webapps                                        # the group to run as (*)
GROUP_ID=996
NUM_WORKERS=5                                        # how many worker processes should Gunicorn spawn (*)
PID_FILE=/tmp/gtracker-runworkers_master.pid
ENVIROMENT=/home/jefferson/.virtualenvs/gtracker

echo "Starting $NAME as `whoami`"

# Activate the virtual environment
cd $DJANGODIR
source $ENVIROMENT/bin/activate
export PYTHONPATH=$DJANGODIR:$PYTHONPATH

python manage.py runworker
