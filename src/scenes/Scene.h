#pragma once

#include <string>
#include <vector>
#include "Object.h"

namespace Octo
{
    class Scene
    {
        public:
            Scene(std::string name = "");

            void createObject();
            void removeObject();

            void update(double delta);
        private:
            std::string m_Name;
            std::vector<Object> m_Objects;
    };
};