/***********************************************************************
                                 Tzy ASCII Paint
                    Copyright 2021 - 2022 Alessandro Salerno

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


#define EXPERIMENTAL_FEATURES
#define NO_REFRESH_OPTIMIZATION
#include "engine/engine.hpp"


namespace Program
{
    static color   tint;
    static coord   x;
    static coord   y;
};


namespace Input
{
    void WaitForAlt()
    {
        while (atkGetKeyState(VK_MENU))
            continue;
    }

    void MoveUp(Engine::Instance* instance, Engine::Clock* clock)
    {
        Program::y -= Program::y > 1;
        WaitForAlt();
    }

    void MoveDown(Engine::Instance* instance, Engine::Clock* clock)
    {
        Program::y += instance->world->height > Program::y + 1;
        WaitForAlt();
    }

    void MoveLeft(Engine::Instance* instance, Engine::Clock* clock)
    {
        Program::x -= Program::x > 0;
        WaitForAlt();
    }

    void MoveRight(Engine::Instance* instance, Engine::Clock* clock)
    {
        Program::x += instance->world->width > Program::x + 1;
        WaitForAlt();
    }

    void Brush(Engine::Instance* instance, Engine::Clock* clock)
    {
        instance->Place(Program::x, Program::y, AGL_EMPTY_CHAR, Program::tint.fgcolor, Program::tint.fgcolor);
    }

    void Delete(Engine::Instance* instance, Engine::Clock* clock)
    {
        instance->Place(Program::x, Program::y, AGL_EMPTY_CHAR, Program::tint.bgcolor, Program::tint.bgcolor);
    }

    void ClearAll(Engine::Instance* instance, Engine::Clock* clock)
    {
        instance->ClearWorld(Program::tint.bgcolor);
    }

    void Type(Engine::Instance* instance, Engine::Clock* clock, uint32 key)
    {
        instance->Place(Program::x, Program::y, (pixel)(key), Program::tint.fgcolor, Program::tint.bgcolor);
        MoveRight(instance, clock);
        atkWaitMills(30);
    }

    void TypingHandler(Engine::Instance* instance, Engine::Clock* clock)
    {
        for (uint32 key = 0x41; key <= 0x5A; key++)
            if (atkGetKeyState(key))
                Type(instance, clock, key);
    }

    void DrawLine(Engine::Instance* instance, Engine::Clock* clock)
    {
        if (atkGetKeyState(VK_UP))
            for (uint64 y = 1; y < instance->world->height; y++)
                instance->Place(Program::x, y, AGL_EMPTY_CHAR, Program::tint.fgcolor, Program::tint.fgcolor);
        
        if (atkGetKeyState(VK_RIGHT))
            for (uint64 x = 0; x < instance->world->width; x++)
                instance->Place(x, Program::y, AGL_EMPTY_CHAR, Program::tint.fgcolor, Program::tint.fgcolor);
    }
}


namespace AsciiPaint
{
    void Render(Engine::Instance* instance, Engine::Clock* clock)
    {
        for (uint64 x = 0; x < instance->buffer->width; x++)
            aglSetCell(instance->buffer, x, 0, AGL_EMPTY_CHAR, Program::tint.fgcolor, Program::tint.fgcolor);

        aglSetCell(instance->buffer, Program::x, Program::y, AGL_EMPTY_CHAR, Program::tint.fgcolor, Program::tint.fgcolor);
    }
}


void OnSetup(Engine::Instance* instance, Engine::Clock* clock)
{
    // Clear background
    instance->ClearWorld(Program::tint.bgcolor);

    // Push inputs
    clock->PushHandler(VK_ESCAPE, OnClose);
    clock->PushHandler(VK_UP, Input::MoveUp);
    clock->PushHandler(VK_DOWN, Input::MoveDown);
    clock->PushHandler(VK_LEFT, Input::MoveLeft);
    clock->PushHandler(VK_RIGHT, Input::MoveRight);
    clock->PushHandler(VK_SPACE, Input::MoveRight);
    clock->PushHandler(VK_RETURN, Input::Brush);
    clock->PushHandler(VK_BACK, Input::MoveLeft);
    clock->PushHandler(VK_DELETE, Input::Delete);
    clock->PushHandler(VK_F10, Input::ClearAll);
    clock->PushHandler(VK_CONTROL, Input::DrawLine);
    clock->Schedule(Input::TypingHandler);

    // Setup rendering functions
    clock->Schedule(AsciiPaint::Render);
}

void OnClose(Engine::Instance* instance, Engine::Clock* clock)
{
    atkEnd(instance->buffer);
}
