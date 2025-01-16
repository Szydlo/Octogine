#include "Entity.h"

using Octo::Entity;

Entity::Entity(std::string name)
    : m_Name(name)
{
    
}

void Entity::update(double deltaTime)
{
    for (auto child : m_Children)
    {
        child->update(deltaTime);
    }
}
