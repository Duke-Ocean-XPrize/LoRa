/* File: lora.i */
%module lora
%{
#define SWIG_FILE_WITH_INIT
#include "src/lora.hpp"
%}
void get_loc(char* outStr);