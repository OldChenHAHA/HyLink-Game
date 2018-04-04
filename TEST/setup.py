# file: setup.py
from distutils.core import setup, Extension

module1 = Extension('mymath',
                    sources = ['mymath.c'])

setup(name = 'mymath',
    version = '1.0',
    description = 'This is a Math package',
    ext_modules= [module1])

