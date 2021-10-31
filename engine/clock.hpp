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


#ifndef CLOCK_H
#define CLOCK_H

    #include "../lib/asciiGL/src/asciigl.h"
    #include "../lib/asciiGL/atk/utils.h"
    #include "../lib/mystd/dictionary.hpp"
    #include "instance.hpp"


    namespace Engine
    {
        class Clock
        {
            private:
                mystd::Vector<void (*)(Engine::Instance*, Engine::Clock*)> _Functions;
                mystd::Dictionary<uint32, void (*)(Engine::Instance*, Engine::Clock*)> _InputHandlers;
            
            public:
                // Couldn't be bothered to actually make a working constructor
                void Setup()
                {
                    this->_Functions    = mystd::Vector<void (*)(Engine::Instance*, Engine::Clock*)>(32);
                    this->_InputHandlers = mystd::Dictionary<uint32, void (*)(Engine::Instance*, Engine::Clock*)>();
                }

                // Pushes a function pointer into the vector of scheduled functions
                void Schedule(void (*function)(Engine::Instance*, Engine::Clock*))
                {
                    this->_Functions.Push(function);
                }

                // Pushes a function pointer into the input handler dictionary alongside the key to activate it
                void PushHandler(uint32 key, void (*handler)(Engine::Instance*, Engine::Clock*))
                {
                    this->_InputHandlers.SetItem(key, handler);
                }

                void Tick(Engine::Instance* instance)
                {
                    // Call all schedule functions
                    for (uint64 i = 0; i < this->_Functions.len; i++)
                        this->_Functions.Get(i)(instance, this);

                    // Check for input and call all necessary input handlers
                    for (uint64 i = 0; i < this->_InputHandlers.GetKeys().len; i++)
                        if (atkIsKeyPressed(this->_InputHandlers.GetKeys().Get(i)))
                            this->_InputHandlers.GetItem(this->_InputHandlers.GetKeys().Get(i))(instance, this);
                }
        };
    }

#endif
