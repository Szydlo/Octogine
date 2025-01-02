#pragma once

#include <string>
#include <vector>

namespace Octo
{
    class Object
    {
        public:
            Object(std::string name = "");

            virtual void update(double deltaTime);
        protected:
            std::vector<Object> m_Children;
            std::string m_Name;
    };
};