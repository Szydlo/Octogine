#include "Object.h"

using Octo::Object;

Object::Object(std::string name)
    : m_Name(name)
{

}

void Object::update(double deltaTime)
{
    for (auto& child : m_Children)
    {
        child.update(deltaTime);
    }
}