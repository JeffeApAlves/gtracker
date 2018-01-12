from setuptools import setup

setup(
    name='gtracker',
    version='0.1',
    py_modules=[
        'project',
        'platform',
        'cl_bash',
        'repository',
        'vanet'    
    ],
    install_requires=[
        'Click',
        'pythondialog',
        'python-nmap',
        'wget',
        'nmap',
        'json'
    ],
    entry_points='''
        [console_scripts]
        app-manage=app-manage:cli
        vanet=vanet:cli
        platform=platform:cli
    ''',
)