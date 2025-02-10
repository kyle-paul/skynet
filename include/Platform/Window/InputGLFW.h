#include "Input.h"

namespace Skynet
{
    class InputGLFW : public Input
    {
    protected:
        virtual bool IsKeyPressedImpl(uint16_t keycode) override;
        virtual bool IsMouseButtonPressedImpl(uint16_t button) override;

        virtual float GetMouseXImpl() override;
        virtual float GetMouseYImpl() override;

        virtual std::pair<float,float> GetMousePosImpl() override;
    };

} // namespace Skynet
