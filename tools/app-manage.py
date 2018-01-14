#!/usr/bin/env python

"""

@file    app-gtracker.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import locale
import sys
import getpass
import subprocess
import click
from distutils import *
from project import *
from cl_bash import *

locale.setlocale(locale.LC_ALL, '')


@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug

@cli.command()
def update():

    '''Atualiza os pacotes do ambiente Python'''

    # arquivo  temporario com a lista  dos pacotes python 
    
    packges='/tmp/env_packages.txt'

    cl = 'pip freeze --local > {0} && pip install -U -r {0}'.format(packges)
    args = shlex.split(cl)
    subprocess.call(args)

    cl = 'pip freeze --local > %s' % (PROJECT.REQUERIMENTS_FILE)    
    args = shlex.split(cl)
    subprocess.call(args)


@cli.command()
def install():

    '''Instala os pacotes do ambiente Python'''
   
    cl = 'pip install -r %s' % (PROJECT.REQUERIMENTS_FILE)
    args = shlex.split(cl)
    subprocess.call(args)


@cli.command()
def deploy():

    '''Copia os arquivos para o servidor'''

    cl = 'rsync -avz %s %s@%s:%s' % (PROJECT.TOOLDIR,getpass.getuser(),WEBSERVER.HOST,WEBSERVER.HOMEDIR)
    args = shlex.split(cl)
    subprocess.call(args)
    
    cl = 'rsync -avz %s %s@%s:%s' % (PROJECT.WEBDIR,getpass.getuser(),WEBSERVER.HOST,WEBSERVER.HOMEDIR)
    args = shlex.split(cl)
    subprocess.call(args)
    
    cl = 'rsync -avz %s/%s.conf %s@%s:%s' % (PROJECT.HOMEDIR,PROJECT.NAME,getpass.getuser(),PROJECT.DEPLOYDIR)
    args = shlex.split(cl)
    subprocess.call(args)

@cli.command()
def config():
   
    '''Configurações referente a parte web que serão salvas na configuração do projeto'''
    pass

@cli.command()
@click.option('--develop/--no-develop',default=True)
@click.option('--worker/--no-worker',default=False)
@click.option('--host',default=WEBSERVER.HOST)
@click.option('--port',default=WEBSERVER.PORT)
def run(worker,host,port,develop):

    '''Executa a aplicação django'''

    if develop:
        dir = PROJECT.WEBDIR
    else:
        dir = WEBSERVER.WEBDIR

    if worker :
        cl = "python %s/manage.py runworker" % (dir)   
    else:
        cl = "python %s/manage.py runserver  %s:%s" % (dir,host,port)
         
    args = shlex.split(cl)
    subprocess.call(args)

if __name__ == '__main__':
    cli(obj={})
