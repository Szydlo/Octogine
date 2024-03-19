#pragma once 

#include <sigslot/signal.hpp>

struct Events 
{
    inline static sigslot::signal<> onStart;
    inline static sigslot::signal<double> onUpdate;
    
};