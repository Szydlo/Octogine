#include "Entity.h"

using Octo::Entity;

Entity::Entity(std::string name, std::shared_ptr<Entity> parent)
    : m_Name(name), m_Parent(parent)
{

}

void Entity::update(double deltaTime)
{
    for (auto& child : m_Children)
    {
        child.update(deltaTime);
    }
}

void Entity::setParent(std::shared_ptr<Entity> parent)
{
    m_Parent = parent;
}