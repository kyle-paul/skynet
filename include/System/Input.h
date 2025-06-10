#ifndef SKYNET_INPUT_H
#define SKYNET_INPUT_H

#include "System.h"

namespace Skynet 
{
    class Input {
    public:
        virtual ~Input() = default;

        static bool IsKeyPressed(uint16_t keycode) { return instance->IsKeyPressedImpl(keycode); }
        static bool IsMouseButtonPressed(uint16_t button) { return instance->IsMouseButtonPressedImpl(button); }

        static bool GetMouseX() { return instance->GetMouseXImpl(); }
        static bool GetMouseY() { return instance->GetMouseYImpl(); }

        static std::pair<float, float> GetMousePos() { return instance->GetMousePosImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(uint16_t keycode) = 0;
        virtual bool IsMouseButtonPressedImpl(uint16_t keycode) = 0;
        
        virtual float GetMouseXImpl() = 0;
        virtual float GetMouseYImpl() = 0;
    
        virtual std::pair<float, float> GetMousePosImpl() = 0;

    private:
        static Input* instance;
    };
}

#endif // SKYNET_INPUT_H