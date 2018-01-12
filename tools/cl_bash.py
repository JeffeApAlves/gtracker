#!/usr/bin/env python

"""

@file    cl_bash.py
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
import xml.etree.ElementTree as ET
from distutils import *
from dialog import Dialog
from project import *

locale.setlocale(locale.LC_ALL, '')


# Dummy context manager to make sure the debug file is closed on exit, be it
# normal or abnormal, and to avoid having two code paths, one for normal mode
# and one for debug mode.
class DummyContextManager:
    def __enter__(self):
        return self

    def __exit__(self, *exc):
        return False


class bash:

    @staticmethod
    def execute_remote(command_line,user,machine):

        bash_cmd = "ssh %s@%s %s" % (user,machine,command_line)
        execute(bash_cmd)

        
    @staticmethod
    def execute(command_line,text=""):

        d = Dialog(dialog="dialog")
        d.set_background_title("Gerenciador do %s" % PROJECT.NAME)

        try:
            devnull = subprocess.DEVNULL
        except AttributeError: # Python < 3.3
            devnull_context = devnull = open(os.devnull, "wb")
        else:
            devnull_context = DummyContextManager()

        args = shlex.split(command_line)

        with devnull_context:
            p = subprocess.Popen(args, stdout=subprocess.PIPE,
                                    stderr=devnull, close_fds=True)
            # One could use title=... instead of text=... to put the text
            # in the title bar.
            d.programbox(fd=p.stdout.fileno(),text=text,height=30,width=150)
            retcode = p.wait()

        # Context manager support for subprocess.Popen objects requires
        # Python 3.2 or later.
        p.stdout.close()
