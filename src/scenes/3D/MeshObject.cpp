#include "MeshObject.h"

using Octo::MeshObject;

MeshObject::MeshObject(std::string name, std::string meshPath)
    : Object3D(name)
{
    m_CurrentModel = new Octo::Model(meshPath);
}

void MeshObject::update(double deltaTime)
{
    Object3D::update(deltaTime);

    m_CurrentModel->setTransform(getTransform());
    m_CurrentModel->draw();
}