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
@click.pass_context
def update(ctx):

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

    cl = 'rsync -avz %s %s@%s' % (PROJECT.TOOLDIR,getpass.getuser(),PROJECT.DEPLOYDIR)
    args = shlex.split(cl)
    subprocess.call(args)
    
    cl = 'rsync -avz %s %s@%s' % (PROJECT.WEBDIR,getpass.getuser(),PROJECT.DEPLOYDIR)
    args = shlex.split(cl)
    subprocess.call(args)

    
    cl = 'rsync -avz %s %s@%s' % ("%s/%s.conf" % (PROJECT.HOMEDIR,PROJECT.NAME),getpass.getuser(),PROJECT.DEPLOYDIR)
    
    click.echo(cl)
    args = shlex.split(cl)
    subprocess.call(args)

@cli.command()
def config():
   
   '''Configurações referente a parte web para serem salvas na configuração do projeto'''
    pass

@cli.command()
@click.option('--worker/--no-worker',default=False)
def run(worker):

    '''Executa a aplicação django'''

    if worker :
        cl = "python %s/manage.py runworker" % PROJECT.WEBDIR   
    else:
        cl = "python %s/manage.py runserver  %s:%s" % (PROJECT.WEBDIR,WEBSERVER.IP,WEBSERVER.PORT)
         
    args = shlex.split(cl)
    subprocess.call(args)

if __name__ == '__main__':
    cli(obj={})
