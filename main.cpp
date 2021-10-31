/***********************************************************************
                                 Tzy ASCII Paint
                        Copyright 2021 Alessandro Salerno

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
***********************************************************************/


#include "paint.hpp"
#include <string.h>


int main(int argc, const char* argv[])
{
    Program::tint.fgcolor = White;
    Program::tint.bgcolor = Black;
    Program::x            = 0;
    Program::y            = 0;
    
    if (argc > 1) Program::tint.fgcolor = (color_t)(atoi(argv[1]));
    if (argc > 2) Program::tint.bgcolor = (color_t)(atoi(argv[2]));
    if (argc > 3) Program::x            = (coord)(atoi(argv[3]));
    if (argc > 4) Program::y            = (coord)(atoi(argv[4]));
    
    Engine::Setup("ASCII Paint");
}
