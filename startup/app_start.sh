#!/bin/bash

NAME="gtracker"                                      #Name of the application (*)
DJANGODIR=/var/www/gtracker/web                      # Django project directory (*)
SOCKFILE=/var/www/gtracker/run/gtracker.sock         # we will communicate using this unix socket (*)
USER=gtracker                                        # the user to run as (*)
USER_ID=999
GROUP=webapps                                        # the group to run as (*)
GROUP_ID=996
NUM_WORKERS=5                                        # how many worker processes should Gunicorn spawn (*)
DJANGO_SETTINGS_MODULE=GoodsTracker.settings         # which settings file should Django use (*)
DJANGO_WSGI_MODULE=GoodsTracker.wsgi                 # WSGI module name (*)
ADDRESS=gtracker.com:8010
PID_FILE=/tmp/gtracker_master.pid
ENVIROMENT=/home/jefferson/.virtualenvs/gtracker


echo "Starting $NAME as `whoami`"

# Activate the virtual environment
#cd $DJANGODIR
source $ENVIROMENT/bin/activate
export DJANGO_SETTINGS_MODULE=$DJANGO_SETTINGS_MODULE
export PYTHONPATH=$DJANGODIR:$PYTHONPATH

# Create the run directory if it doesn't exist
RUNDIR=$(dirname $SOCKFILE)
test -d $RUNDIR || mkdir -p $RUNDIR

#Start your Djangos uwsgi
uwsgi --chdir=$DJANGODIR \
    --module=${DJANGO_WSGI_MODULE}:application \
    --env DJANGO_SETTINGS_MODULE=$DJANGO_SETTINGS_MODULE \
    --master --pidfile=$PID_FILE  \
    --socket=$SOCKFILE \
    --chmod-socket=666 \
    --processes=$NUM_WORKERS \
    --uid=$USER --gid=$GROUP \
    --harakiri=20 \
    --max-requests=5000 \
    --logger file:/var/log/gtracker/uwsgi.log \
    --vacuum \
    --http-websockets \
    --home=$ENVIROMENT
 #--daemonize=/var/log/uwsgi/yourproject.log