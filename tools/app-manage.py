#!/usr/bin/env python
#!-*- conding: utf8 -*-

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
@click.option('--production/--no-production',default=False)
@click.pass_context
def cli(ctx, debug, production):
    ctx.obj['DEBUG'] = debug
    ctx.obj['PRODUCTION'] = production


@cli.command()
def update():

    '''Atualiza os pacotes do ambiente Python'''

    # arquivo  temporario com a lista  dos pacotes python 
    packges='/tmp/env_packages.txt'

    with open(packges,'w') as f:

        # mapeia todos o pacotes utilizados
        cl = "pip freeze --local"    
        args = shlex.split(cl)
        subprocess.call(args,stdout = f)

        # atualiza conforme mapeamento
        cl = "pip install -U -r %s" % (packges)
        args = shlex.split(cl)
        subprocess.call(args)

    with open(PROJECT.REQUERIMENTS_FILE,'w') as req:

        # Atualiza o arquivo do projeto 
        cl = "pip freeze --local"    
        args = shlex.split(cl)
        subprocess.call(args,stdout = req)


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
 

    cl = 'rsync -rv  --include="*.conf" --exclude="*" --prune-empty-dirs %s/ %s@%s:%s' % (PROJECT.HOMEDIR,getpass.getuser(),WEBSERVER.HOST,WEBSERVER.HOMEDIR)
    click.echo(cl)
    args = shlex.split(cl)
    subprocess.call(args)

    cl = 'rsync -rv  --include="*.sh" --exclude="*" --prune-empty-dirs %s/ %s@%s:%s' % (PROJECT.HOMEDIR,getpass.getuser(),WEBSERVER.HOST,WEBSERVER.HOMEDIR)
    click.echo(cl)
    args = shlex.split(cl)
    subprocess.call(args)


@cli.command()
def config():

    '''Configurações referente a parte web que serão salvas na configuração do projeto'''
    pass


@cli.command()
@click.pass_context
def static_files(ctx):
    
    '''Coleta os arquivos estaticos do projeto para o nginx servi-los'''
    
    if ctx.obj['PRODUCTION']:
        workdir = WEBSERVER.WEBDIR
    else:
        workdir = PROJECT.WEBDIR

    cl = "python %s/manage.py collectstatic" % (workdir)   
      
    args = shlex.split(cl)
    subprocess.call(args)


@cli.command()
@click.option('--worker/--no-worker',default=False)
@click.option('--host',default=WEBSERVER.HOST)
@click.option('--port',default=WEBSERVER.PORT)
@click.pass_context
def run(ctx,worker,host,port):

    '''Executa a aplicação django'''

    if ctx.obj['PRODUCTION']:
        workdir = WEBSERVER.WEBDIR
    else:
        workdir = PROJECT.WEBDIR
 
    if worker :
        cl = "python %s/manage.py runworker" % (workdir)   
    else:
        cl = "python %s/manage.py runserver  %s:%s" % (workdir,host,port)
         
    args = shlex.split(cl)
    subprocess.call(args)


if __name__ == '__main__':
    cli(obj={})
