#ifndef SKYNET_TIMESTEP_H
#define SKYNET_TIMESTEP_H

namespace Skynet
{
    class Timestep 
    {
    public:
        Timestep(float time = 0.0f) : time(time) { }
        operator float() const { return time; }
        
        float GetSeconds() const { return time; }
        float GetMilliSeconds() const { return time * 1000; }
    
    private:
        float time;
    };
}

#endif // SKYNET_TIMESTEP_H