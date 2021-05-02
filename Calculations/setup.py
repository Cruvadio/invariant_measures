from distutils.core import setup, Extension, DEBUG


INCLUDE_PATH = 'C:\\Users\\Death\\source\\repos\\Symbol_2\\packages\\boost.1.76.0.0\\lib\\native\\include'
LIBRARY_DIR = 'C:\\Users\\Death\\source\\repos\\Symbol_2\\Release'

sfc_module = Extension('calculations',
                       define_macros=[('MAJOR_VERSION', '0'), ('MINOR_VERSION', '1')],
                      include_dirs=[INCLUDE_PATH],
                      libraries=['calculations'],
                      library_dirs=[LIBRARY_DIR],
                      sources = ['module.cpp', 'SymbolicImage.cpp'])

setup(name = 'calculations', version = '1.0',
    description = 'Python Package with superfastcode C++ extension',
    ext_modules = [sfc_module]
    )