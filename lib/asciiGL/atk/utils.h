/***********************************************************************
                          asciiGL Toolkit
               Copyright 2021 asciiGL Toolkit contributors

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


#ifndef ATK_UTILS_H
#define ATK_UTILS_H

    #include "../src/asciigl.h"
    #include "colors.h"
    #include <stdlib.h>


    // Wait n. milliseconds
    void atkWaitMills(unsigned int mills)
    {
        #ifdef _WIN32
            Sleep(mills);
        #else
            usleep(mills * 1000);
        #endif
    }

    // Change window title
    void atkSetWindowTitle(const char* title)
    {
        #ifdef _WIN32
            SetConsoleTitle(title);
        #else
            printf("\033]0;%s\007", title);
        #endif
    }
    
    // Reset console, cursor and exit
    void atkEndProgram(int signum)
    {
        consoleClearScreen();
        consoleRestoreCursorPosition();
        consoleShowCursor();
        exit(0);
    }

    // Call 'ResetConsole' when CTRL-C is pressed
    void atkInitInterrupt()
    {
        signal(SIGINT, atkEndProgram);
    }

    // Returns the width of the console
    unsigned int atkGetConsoleWidth()
    {
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            
            return csbi.srWindow.Right - csbi.srWindow.Left + 1;
        #else
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
            return size.ws_col;
        #endif
    }

    // Returns the height of the console
    unsigned int atkGetConsoleHeight()
    {
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
            
            return csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
        #else
            struct winsize size;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
            return size.ws_row;
        #endif
    }

    // Creates and returns a framebuffer with the same size as the window
    framebuffer atkSetup()
    {
        unsigned int width  = atkGetConsoleWidth(),
                     height = atkGetConsoleHeight() - 1;

        return Framebuffer(width, height);
    }

    // Init framebuffer and event listener
    void atkInit(framebuffer buffer)
    {
        aglInitContext(buffer);
        atkInitInterrupt();
    }

    // automatically resizes the framebuffer if the window has been resized
    bool atkAutoResize(framebuffer buffer)
    {
        unsigned int width  = atkGetConsoleWidth(),
                     height = atkGetConsoleHeight() - 1;

        if (buffer->width == width && buffer->height == height)
            return false;

        aglResizeFramebuffer(buffer, width, height);
        aglClear(buffer, ' ', Black, Black);
        consoleClearScreen();
        consoleHideCursor();
        aglDrawFramebuffer(buffer);
        
        return true;
    }

    // Terminates everything
    void atkEnd(framebuffer buffer)
    {
        aglEndContext(buffer);
        atkEndProgram(0);
    }

    // Experimental async input handler
    #if defined (_WIN32) && defined (EXPERIMENTAL_FEATURES)
        short atkIsKeyPressed(int key)
        {
            return GetAsyncKeyState(key);
        }
    #else
        short atkIsKeyPressed(int key)
        {
        }
    #endif

#endif