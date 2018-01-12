#!/usr/bin/env python

"""

@file    project.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
import json
import os.path
import locale
import tarfile
import sys
import ast
import getpass
import subprocess
import click
import shlex
import nmap
import wget
import distutils
import xml.etree.ElementTree as ET
from distutils import *
from dialog import Dialog

locale.setlocale(locale.LC_ALL, '')


@click.group()
@click.option('--debug/--no-debug', default=False)
@click.pass_context
def cli(ctx, debug):
    ctx.obj['DEBUG'] = debug


class PROJECT:

    PROJECT_CONF = os.getenv('PROJECT_CONF','./project.conf')

    with open(PROJECT_CONF) as json_data_file:
        configuration = json.load(json_data_file)
        
    # nome do projeto
    NAME = configuration['project']['name']

    # Estrutura de diretorios da solucao

    # diretrio raiz da solução
    HOMEDIR = configuration['project']['home']
    #HOMEDIR = os.environ.get('PROJECT_HOME',os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..'))
    # ferramnetas
    TOOLDIR=HOMEDIR + '/tools'
    # site
    WEBDIR = HOMEDIR + '/web' 
    # embarcado
    PLATFORMDIR = HOMEDIR + '/platform'
    # Diretorio da simulação
    VANETDIR=HOMEDIR + '/vanet' 
    # local para deploy da solução
    #DEPLOYDIR=os.getenv('DEPLOYDIR','/var/wwww/' + NAME)
    DEPLOYDIR= "%s/%s" % (configuration['project']['deploy'],NAME)
    # Arquivos
    REQUERIMENTS_FILE= WEBDIR + "/requirements.txt"

    # adiciona diretorio de ferramentas no path
    sys.path.append(os.path.join(HOMEDIR, 'tools'))


class WEBSERVER:

    # IP do webserver da solução
    IP = PROJECT.configuration['webserver']['host']
    #IP = os.getenv('IP_WEBSERVER','localhost')
    # porta do webserver da solução
    #PORT = os.getenv('PORT_WEBSERVER','8000')
    PORT = PROJECT.configuration['webserver']['port']