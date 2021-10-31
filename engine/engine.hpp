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


#ifndef ENGINE_H
#define ENGINE_H

    #include "instance.hpp"
    #include "clock.hpp"

    // Declares two function prototypes for later
    void OnSetup(Engine::Instance* instance, Engine::Clock* clock);
    void OnClose(Engine::Instance* instance, Engine::Clock* clock);

    namespace Engine
    {
        // Terminates the session
        void Close(Engine::Instance* instance, Engine::Clock* clock)
        {
            OnClose(instance, clock);
            atkEnd(instance->buffer);
        }

        // Initiates the session
        void Setup(const char* title)
        {
            // Sets the window title
            atkSetWindowTitle(title);

            // Initializes the engine
            Engine::Instance* instance = (Engine::Instance*)(malloc(sizeof(Engine::Instance)));
            instance->buffer  = atkSetup();
            instance->world = Framebuffer(instance->buffer->width, instance->buffer->height);

            // Initializes a clock
            Engine::Clock* clock = (Engine::Clock*)(malloc(sizeof(Engine::Clock)));
            clock->Setup();

            // Calls the user-defined setup function
            OnSetup(instance, clock);

            // Initializes the console
            atkInit(instance->buffer);

            // Runs once every frame
            while (true)
            {
                // Calls scheduled functions and input handlers
                clock->Tick(instance);

                // Draws the final frame to the screen
                aglSwapBuffers(instance->buffer);
            }
        }
    }

#endif