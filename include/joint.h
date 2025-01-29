#pragma once

#include "transformable.h"

class Joint : public Transformable
{
public:
    Joint(float w[3]);
    ~Joint();
};