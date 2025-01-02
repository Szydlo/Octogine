#include "Scene.h"

using Octo::Scene;

Scene::Scene(std::string name)
    : m_Name(name)
{

}

void Scene::update(double deltaTime)
{
    for (auto& object : m_Objects)
    {
        object.update(deltaTime);
    }
}