from setuptools import setup

setup(
    name='gtracker',
    version='0.1',
    py_modules=['gtracker'],
    install_requires=[
        'Click',
    ],
    entry_points='''
        [console_scripts]
        gtracker=gtracker:cli
    ''',
)