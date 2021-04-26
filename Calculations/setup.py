from distutils.core import setup, Extension, DEBUG

sfc_module = Extension('calculations', sources = ['module.cpp'])

setup(name = 'calculations', version = '1.0',
    description = 'Python Package with superfastcode C++ extension',
    ext_modules = [sfc_module]
    )