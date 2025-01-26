#include "Entity.h"

using Octo::Entity;

Entity::Entity(std::string name)
    : m_Name(name)
{
    Events::onNodeStart.connect(&Entity::onStart, this);
    Events::onUpdate.connect(&Entity::onUpdate, this);
}

void Entity::onStart()
{

}

void Entity::onUpdate(double deltaTime)
{
    for (auto child : m_Children)
    {
        child->onUpdate(deltaTime);
    }
}
