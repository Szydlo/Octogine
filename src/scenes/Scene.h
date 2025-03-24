#pragma once

#include "Entity.h"

namespace Octo
{
    class Scene : public Entity
    {
        public:
        explicit Scene(const std::string &name = "");
    };
};