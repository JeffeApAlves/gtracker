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
import sys
from distutils import *

locale.setlocale(locale.LC_ALL, '')

class PROJECT(object):

    PROJECT_CONF = os.getenv('PROJECT_CONF','./project.conf')

    with open(PROJECT_CONF) as json_data_file:
        configuration = json.load(json_data_file)
        
    # nome do projeto
    NAME = configuration['project']['name']

    # Estrutura de diretorios da solucao

    # diretrio raiz da solução
    HOMEDIR = configuration['project']['home']
    # ferramnetas
    TOOLDIR=HOMEDIR + '/tools'
    # diretorio do site
    WEBDIR = HOMEDIR + '/web' 
    # embarcado
    PLATFORMDIR = HOMEDIR + '/platform'
    # Diretorio da simulação
    VANETDIR=HOMEDIR + '/vanet' 
    # local para deploy da solução
    # Arquivos
    REQUERIMENTS_FILE= WEBDIR + "/requirements.txt"

    # adiciona diretorio de ferramentas no path
    sys.path.append(os.path.join(HOMEDIR, 'tools'))


class WEBSERVER(object):

    # IP do webserver da solução
    HOST = PROJECT.configuration['webserver']['host']
    # porta do webserver da solução
    PORT = PROJECT.configuration['webserver']['port']
    # diretrio raiz da solução
    HOMEDIR = PROJECT.configuration['webserver']['home']
    # diretorio do site
    WEBDIR = HOMEDIR + '/web' 