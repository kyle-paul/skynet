#ifndef SKYNET_FRAME_H
#define SKYNET_FRAME_H

#include "System.h"
#include "glad/glad.h"

namespace Skynet
{
    
    class Frame
    {
    public:
        Frame();
        ~Frame();

        void Init();
        void Bind() const;
        void Unbind() const;

        inline uint32_t GetWidth() const { return width; }
        inline uint32_t GetHeight() const { return height; }
        inline uint32_t GetColor() const { return color; }

        void Resize(uint32_t &width, uint32_t &height);

    private:
        uint32_t fbo, color, depth;

        uint32_t width = 1500;
        uint32_t height = 900;

        friend class Interface;
    };

} // namespace Skynet

#endif // SKYNET_FRAME_H

