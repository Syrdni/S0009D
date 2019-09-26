#pragma once

#include <string>
#include <cstring>
#include <iostream>
#include <fstream>
#include "nax3Structs.h"
#include "animationResource.h"
#include "Vector4D.h"

class Animator
{
    public:
        Animator();
        ~Animator();
        bool readNax3File(std::string fileLocation);
};