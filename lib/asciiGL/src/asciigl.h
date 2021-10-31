/***********************************************************************
                                     asciiGL
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


#ifndef ASCIIGL_H
#define ASCIIGL_H

    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #ifndef EXPERIMENTAL_FEATURES
        #ifdef LEGACY_CONSOLE
            #undef LEGACY_CONSOLE
            #warning "Ignoring LEGACY_CONSOLE due to lack of EXPERIMENTAL_FEATURES"
        #endif
    #endif

    #ifdef _WIN32
        #include <windows.h>
        #include <signal.h>

        #ifdef LEGACY_CONSOLE
            #warning "Legacy Console Mode (LEGACY_CONSOLE) is enabled, this may result in slower rendering and reduced color depth!"
        #endif
    #else
        #define _POSIX_SOURCE
        #include <unistd.h>
        #include <sys/ioctl.h>
    #endif

    #define AGL_EMPTY_CHAR ' '
    
    
    #pragma region CONSOLE
        void consoleMoveCursor(int x, int y)
        {
            #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                COORD coord;
                coord.X = x + 1;
                coord.Y = y + 1;
                SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
            #else
                printf("\033[%d;%dH", y + 1, x + 1);
            #endif
        }

        void consoleRestoreCursorPosition()
        {
            printf("\033[k");
        }

        void consoleHideCursor()
        {
            #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_CURSOR_INFO info;
                info.dwSize = 100;
                info.bVisible = FALSE;
                SetConsoleCursorInfo(consoleHandle, &info);
            #else
                printf("\e[?25l");
            #endif
        }

        void consoleShowCursor()
        {
            #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_CURSOR_INFO info;
                info.dwSize = 100;
                info.bVisible = TRUE;
                SetConsoleCursorInfo(consoleHandle, &info);
            #else
                printf("\e[?25h");
            #endif
        }

        void consoleClearScreen()
        {
            #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                COORD topLeft  = { 0, 0 };
                HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO screen;
                DWORD written;

                GetConsoleScreenBufferInfo(console, &screen);
                FillConsoleOutputCharacterA(
                    console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
                );

                FillConsoleOutputAttribute(
                    console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
                    screen.dwSize.X * screen.dwSize.Y, topLeft, &written
                );

                SetConsoleCursorPosition(console, topLeft);
            #else
                printf("\033c");
            #endif
        }
    #pragma endregion


    #pragma region FRAMEBUFFER
        typedef unsigned char pixel; 
        typedef unsigned int coord;
        typedef unsigned char color_t; 

        typedef struct
        {
            color_t fgcolor;
            color_t bgcolor;
        } color;

        typedef struct
        {
            pixel content;
            color tint;
            bool  update; 
        } cell;

        typedef struct
        {
            unsigned int    width; 
            unsigned int    height; 
                     size_t size;

                     cell*  texture;
                     char*  printbuff;
        } _Framebuffer;

        typedef _Framebuffer* framebuffer;

        static void _InitializeCell(cell* c, pixel content, color_t fgcolor, color_t bgcolor)
        {
            c->content = content;
            c->tint    = (color) { .fgcolor = fgcolor, .bgcolor = bgcolor };
            c->update  = true;
        }

        framebuffer Framebuffer(unsigned int width, unsigned int height)
        {
            framebuffer buffer    = (framebuffer)(malloc(sizeof(_Framebuffer)));

            buffer->width     = width;
            buffer->height    = height;
            buffer->size      = width * height;

            buffer->texture   = (cell*)(malloc(sizeof(cell) * buffer->size));
            buffer->printbuff = (char*)(malloc(buffer->size * 7));

            for (int i = 0 ; i < buffer->size; i++)
                _InitializeCell(&buffer->texture[i], ' ', 255, 0);

            return buffer;
        }
    #pragma endregion


    #pragma region ASCIIGL_FUNCTIONS
        int aglTranslateCoordinates(framebuffer buffer, coord x, coord y)
        {
            return y * buffer->width + x;
        }

        void aglDrawIndex(framebuffer buffer, unsigned int index)
        {
            #ifndef NO_REFRESH_OPTIMIZATION
                if (buffer->texture[index].update)
            #endif
            {
                #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                    WORD wColor = ((buffer->texture[index].tint.bgcolor & 0x0F) << 4) + (buffer->texture[index].tint.fgcolor & 0x0F);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
                #else
                    printf("\033[38;5;%dm\033[48;5;%dm", buffer->texture[index].tint.fgcolor, buffer->texture[index].tint.bgcolor);
                #endif

                printf("%lc", buffer->texture[index].content);
            }
        }
        
        void aglDrawCell(framebuffer buffer, coord x, coord y)
        {
            consoleMoveCursor(x, y);
            aglDrawIndex(buffer, aglTranslateCoordinates(buffer, x, y));
        }
    
        void aglEndDraw(framebuffer buffer)
        {
            for (unsigned int i = 0; i < buffer->size; i++)
                buffer->texture[i].update = false;
        }

        void aglDrawFramebuffer(framebuffer buffer)
        {
            for (coord y = 0; y < buffer->height; y++)
            {
                for (coord x = 0; x < buffer->width; x++)
                    aglDrawIndex(buffer, aglTranslateCoordinates(buffer, x, y));

                printf("\n");
            }

            #ifndef LEGACY_CONSOLE
                aglEndDraw(buffer);
            #endif
        }

        void aglSwapBuffers(framebuffer buffer)
        {
            #if defined (_WIN32) && defined (LEGACY_CONSOLE)
                consoleClearScreen();
                aglDrawFramebuffer(buffer);
            #else
                for (coord y = 0; y < buffer->height; y++)
                    for (coord x = 0; x < buffer->width; x++)
                        aglDrawCell(buffer, x, y);

                aglEndDraw(buffer);
            #endif
        }

        void aglResizeFramebuffer(framebuffer buffer, unsigned int new_width, unsigned int new_height)
        {
            unsigned int idx      = buffer->size; 

            buffer->width         = new_width;
            buffer->height        = new_height;
            buffer->size          = new_width * new_height;

            buffer->texture       = (cell*)(realloc(buffer->texture, buffer->size * sizeof(cell)));
            buffer->printbuff     = (char*)(realloc(buffer->printbuff, buffer->size * 7));

            for (int i = idx ; i < buffer->size; i++)
                _InitializeCell(&buffer->texture[i], ' ', 255, 0);
        }

        void aglSetCell(framebuffer buffer, coord x, coord y, pixel content, color_t fgcolor, color_t bgcolor)
        {
            unsigned int idx = aglTranslateCoordinates(buffer, x, y);
            
            buffer->texture[idx].update        = !(buffer->texture[idx].content == content && buffer->texture[idx].tint.fgcolor == fgcolor && buffer->texture[idx].tint.bgcolor == bgcolor);
            buffer->texture[idx].content       = content;
            buffer->texture[idx].tint.fgcolor  = fgcolor;
            buffer->texture[idx].tint.bgcolor  = bgcolor;
        }

        void aglClear(framebuffer buffer, pixel chr, color_t fgcolor, color_t bgcolor)
        {
            for (coord y = 0; y < buffer->height; y++)
                for (coord x = 0; x < buffer->width; x++)
                    aglSetCell(buffer, x, y, chr, fgcolor, bgcolor);
        }

        void aglInitContext(framebuffer buffer)
        {
            consoleClearScreen();
            consoleHideCursor();
            setvbuf(stdout, buffer->printbuff, _IOFBF, buffer->size * 7);
            aglDrawFramebuffer(buffer);
        }

        void aglEndContext(framebuffer buffer)
        {
            free(buffer->texture);
            free(buffer->printbuff);
            free(buffer);
        }
    #pragma endregion


#endif