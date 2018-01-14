#!/usr/bin/env python

"""

@file    gdb.py
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
from esp32 import *
#from tui_menus import *


class GDB(object):

    # diretŕorio de trabalho     
    HOMEDIR = ESP32.HOMEDIR 

    # arquivo de inicialização
    gdbinit = "%s/%s" % (HOMEDIR , '/gdbinit' )
    
    # repositório do openocd
    openocd_url = PROJECT.configuration["platform"]["esp32"]["gdb"]["openocd_url"]
    
    # diretorório com os arquivos de cfg das interfaces
    interface_path ="/usr/local/share/openocd/scripts/interface"
    
    # diretorório com os arquivos de cfg dos tragets
    target_path = "/usr/local/share/openocd/scripts/target"
    
    # diretorório onde está o executavel para debug
    program_path = "%s/%s" % (HOMEDIR , PROJECT.configuration["platform"]["esp32"]["gdb"]["home"])

    # target selecionando
    target = "%s/%s"  % (target_path,PROJECT.configuration["platform"]["esp32"]["gdb"]["target"])
    
    # interface selecionada
    interface = "%s/%s" % (interface_path, PROJECT.configuration["platform"]["esp32"]["gdb"]["interface"])
    
    # programa para debug
    program = "%s/%s" % (program_path, PROJECT.configuration["platform"]["esp32"]["gdb"]["program"])
    
    # ip do hohst gdb
    host_gdb = PROJECT.configuration["platform"]["esp32"]["gdb"]["host"]
    
    # extensão do arquivo de programa
    ext_program = 'elf'
    
    # extensão dos arquivos de configuração
    ext_config  = 'cfg'
    
    # frequencia de debug
    freq_jtag = PROJECT.configuration["platform"]["esp32"]["gdb"]["freq_jtag"]
    
    # local de instalação do openocd
    openocd_path = PROJECT.configuration["platform"]["esp32"]["gdb"]["openocd_path"]

    transport = PROJECT.configuration["platform"]["esp32"]["gdb"]["transport"]


    def __init__(self):
        pass

    @staticmethod
    def start_debug_section(file_init = gdbinit, program = program):


        if os.path.isfile(file_init):

            if os.path.isfile(program):

                cl = 'xtensa-esp32-elf-gdb -x %s %s' % (file_init,program)

                args = shlex.split(cl)
                
                subprocess.call(args)

            else:
                d = Dialog(dialog="dialog")
                d.msgbox('Não foi possivel abrir o arquivo %s' % program,title="ERRO")
        else:
            d = Dialog(dialog="dialog")
            d.msgbox('Não foi possivel abrir o arquivo %s' % file_init,title="ERRO")


    @staticmethod
    def start_debug_server():

        '''Inicia o openocd utilizando os parametro recebidos'''

        GDB.reset_target()

        cl = "sudo openocd -f %s '- c transport select %s' -f %s -d 3" % (GDB.interface,GDB.transport,GDB.target)

        bash.execute_remote(cl,getpass.getuser(),GDB.host_gdb)


    @staticmethod
    def stop_debug_server():

        '''Finaliza o processo correspondente ao openocd'''
        cl = "sudo pkill openocd"

        bash.execute_remote(cl,getpass.getuser(),GDB.host_gdb)
     
        
    @staticmethod
    def config_jtag(freq):

        GDB.freq_jtag = value
            
        bash.execute_remote("sed -e '/adapter_khz/D' %s > '/tmp/interface'" % GDB.interface)
        bash.execute_remote("echo  'adapter_khz '%s >> '/tmp/interface'" % GDB.freq_jtag)
        bash.execute_remote("sudo  cp '/tmp/interface' %s" % GDB.interface)


    @staticmethod
    def restart():
        GDB.stop_debug_server()
        GDB.start_debug_server()

    @staticmethod
    def shutdown_interface():

        bash.execute_remote("sudo shutdown now",getpass.getuser(),GDB.host_gdb)

    @staticmethod
    def reset_target():

        bash.execute_remote("gpio -g mode 8 out",getpass.getuser(),GDB.host_gdb)
        bash.execute_remote("gpio -g write 8 1",getpass.getuser(),GDB.host_gdb)
        sleep(2.0) 
        bash.execute_remote("gpio -g write 8 0",getpass.getuser(),GDB.host_gdb)
        sleep(0.3)
        bash.execute_remote("gpio -g write 8 1",getpass.getuser(),GDB.host_gdb)
        sleep(1.0)     

    @staticmethod
    def scan_gdbserver():

        #stdout, erro = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE).communicate()
        #cl = "nmap -n -sn 192.168.42.0/24 -oG - | awk '/Up$/{print $2}'"
        #args = shlex.split(cl)

        nm = nmap.PortScanner() 
        nm.scan('192.168.42.0/24', '22-443')      # scan host 127.0.0.1, ports from 22 to 443

        choices = []

        for l in nm.all_hosts():
            choices.append((l,"",False))

        d = Dialog(dialog="dialog")
        #d.set_background_title(Menu.background_title)

        code, tag = d.radiolist("Selecione o IP do Host GDB",choices=choices)

        if code == d.OK:
            GDB.host_gdb = tag