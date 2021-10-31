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


#ifndef INSTANCE_H
#define INSTANCE_H

    #include "../lib/asciiGL/src/asciigl.h"
    #include "../lib/asciiGL/atk/utils.h"
    #include "../lib/mystd/types.hpp"


    namespace Engine
    {
        class Instance
        {
            public:
                framebuffer world;
                framebuffer buffer;

            public:
                // Clears the world buffer
                void ClearWorld(color_t sky)
                {
                    aglClear(this->world, AGL_EMPTY_CHAR, sky, sky);
                }

                // Draws the world onto the rendering buffer
                void DrawWorld()
                {
                   #define content this->world->texture[aglTranslateCoordinates(this->world, x, y)].content
                   #define fgcolor this->world->texture[aglTranslateCoordinates(this->world, x, y)].tint.fgcolor
                   #define bgcolor this->world->texture[aglTranslateCoordinates(this->world, x, y)].tint.bgcolor

                    for (coord y = 0; y < this->world->height; y++)
                        for (coord x = 0; x < this->world->width; x++)
                            aglSetCell(this->buffer, x, y, content, fgcolor, bgcolor);

                    #undef content
                    #undef fgcolor
                    #undef bgcolor
                }

                // Sets a cell of the world buffer
                void Place(coord x, coord y, pixel content, color_t fgcolor, color_t bgcolor)
                {
                    aglSetCell(this->world, x, y, content, fgcolor, bgcolor);
                }
        };
    }

#endif