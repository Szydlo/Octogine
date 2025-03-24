#include "Entity.h"

#include "../core/Events.h"

using Octo::Entity;

Entity::Entity(const std::string &name)
    : m_Name(name)
{
    Events::onNodeStart.connect(&Entity::onStart, this);
    Events::onUpdate.connect(&Entity::onUpdate, this);
}

void Entity::onStart()
{

}

void Entity::onUpdate(const double deltaTime)
{
    for (const auto child : m_Children)
    {
        child->onUpdate(deltaTime);
    }
}
