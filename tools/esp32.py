#!/usr/bin/env python

"""

@file    esp32.py
@author  Jefferson Alves
@date    2018-01-09
@version 0.1


Gerenciador do projeto

"""

import os
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
import xml.etree.ElementTree as ET
from distutils import *
from dialog import Dialog
from project import *
from cl_bash import *
from time import sleep
from tui_menus import *


class ESP32(object):

    # nome do proojeto
    PROJECT_NAME = PROJECT.configuration["platform"]["esp32"]["name"]
    # diretorio do projeto
    HOMEDIR = "%s/%s" % (PROJECT.PLATFORMDIR ,PROJECT_NAME)
    # porta seral
    SERIAL_PORT = PROJECT.configuration["platform"]["esp32"]["serial_port"]
    # local do toolchain para o ESP
    IDF_PATH = os.getenv("IDF_PATH","/home/%s/esp-idf" % getpass.getuser())


    def __init__(self):
        pass

    @staticmethod
    def compile_all():

        os.chdir(ESP32.HOMEDIR)
        subprocess.call(shlex.split("make clean"),stdout = subprocess.DEVNULL , stderr=subprocess.STDOUT)
        bash.execute("make all","Compilando all")

    @staticmethod
    def compile_app():
        
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make app","Compilando app")

    @staticmethod
    def create_project():

        '''Cria um projeto para ESP32 baseado no codigo demo blink''' 

        d = Dialog(dialog="dialog")
        d.set_background_title(Menu.background_title)

        code, value = d.inputbox(text = "Informe a frequencia do jtag em [KHz]")
     
        if code == d.OK:

            PROJECT.NAME = value

            code,path = d.dselect("./",title="Seleção do local do projeto",height=30,width=100)

            if code == d.OK:

                PROJECT.HOMEDIR = path

                copy_tree(ESP32.PROJECT_TEMPLATE,PROJECT.HOMEDIR)
                
                # renomeia o direotiro blink para o nome do projeto
                move_tree(PROJECT.HOMEDIR+'/blinck',PROJECT.HOMEDIR+PROJECT.NAME)

                subprocess.call("sed -e '/PROJECT_NAME/D' '$filepath/$new_project_name/Makefile' > '/tmp/Makefile'")
                subprocess.call("echo  'PROJECT_NAME := '$new_project_name >> '/tmp/Makefile'")
  
                copy_file("/tmp/Makefile","%s/%s" % (PROJECT.HOMEDIR,PROJECT.NAME))


    @staticmethod
    def serial_permission():

        try:
            subprocess.call(shlex.split("sudo chmod 777 %s" % ESP32.SERIAL_PORT),stdout = subprocess.DEVNULL , stderr=subprocess.STDOUT)
        except:
            click.echo("Serial não disponivel")


    @staticmethod
    def flash():

        ESP32.serial_permission()
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make flash","Compilando flash")

    @staticmethod
    def monitor():

        ESP32.serial_permission()
        os.chdir(ESP32.HOMEDIR)
        bash.execute("make monitor","Monitor uart")

    @staticmethod
    def config():
        
        os.chdir(ESP32.HOMEDIR)
        subprocess.call(shlex.split("make menuconfig"))

    @staticmethod
    def manage_sdk():

        try:
            GDB.update_repositorio(GDB.url_sdk,GDB.idf_path)
        except:
            try:
                GDB.clone_repositorio(GDB.url_sdk,GDB.idf_path)
            except:
                click.echo("Não foi possivel clonar/atualizar o sdk")


    @staticmethod
    def manage_toolchain():

        choices = [ ("Versão 64 bits","",True)
                    ("Versão 32 bits","",False)
        ]

        code, tag = d.radiolist("Selecione o IP do Host GDB",choices=choices)

        if code ==d.OK:

            code,path = d.dselect("./",title="Seleção do local do toolchain",height=30,width=100)

            if code == d.OK:
                
                ESP32.IDF_PATH = path
                 
                file_name = ""

                if tag == "Versão 64 bits":
                    file_name = wget.download(ESP32.url_espressif_toolchain64,bar=bar_thermometer)
                else:
                    file_name = wget.download(ESP32.url_espressif_toolchain32,bar=bar_thermometer)


                tar = tarfile.open(file_name)
                tar.extractall(ESP32.IDF_PATH)
                tar.close()

    @staticmethod
    def manage_openocd():

        try:
            GDB.update_repositorio(GDB.openocd_url,GDB.openocd_path,GDB.host_gdb)
        except:
            try:
                GDB.clone_repositorio(GDB.openocd_url,GDB.openocd_path,GDB.host_gdb)
            except:
                click.echo("Não foi possivel clonar/atualizar o OpenOCD")
