#pragma once 

#include <sigslot/signal.hpp>

struct Events 
{
    inline static sigslot::signal<> onStart;
    inline static sigslot::signal<> onNodeStart;
    inline static sigslot::signal<double> onUpdate;
    inline static sigslot::signal<int, bool> onClick;
    inline static sigslot::signal<double, double> onMouseMove;
};