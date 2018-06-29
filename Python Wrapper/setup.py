#!/usr/bin/env python
"""
setup.py file for SWIG example
"""
from distutils.core import setup, Extension
lora_module = Extension('_lora',
sources=['lora_wrap.cxx', 'src/lora.cpp'],
)
setup (name = 'lora',
version = '0.1',
author = "SWIG Docs",
description = """Simple swig lora from docs""",
ext_modules = [lora_module],
py_modules = ["lora"],
)