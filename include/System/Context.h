#ifndef SKYNET_CONTEXT_H
#define SKYNET_CONTEXT_H

namespace Skynet 
{
    class Context 
    {
    public:
        virtual ~Context() = default;
        virtual void Init() = 0;
    };
}

#endif // SKYNET_CONTEXT_H