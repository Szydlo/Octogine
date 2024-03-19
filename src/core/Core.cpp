#include "Core.h"

#include <iostream>

using Octo::Core;

Core::Core() 
{
    spdlog::info("Starting Octogine");

    Events::onStart();

    while(true) 
    {
        Events::onUpdate(0.1);
    }
}