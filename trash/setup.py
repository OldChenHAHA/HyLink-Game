# file: setup.py
from distutils.core import setup, Extension

MOD = 'lcd12864'

module1 = Extension(MOD, sources = ['lcd12864.c'])

setup(name = MOD,
    version = '1.0',
    description = 'This is a lcd package',
    ext_modules= [module1])

