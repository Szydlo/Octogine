#include "Object3D.h"

using Octo::Object3D;

Object3D::Object3D(std::string name)
    : Object(name)
{

}

glm::mat4 Object3D::getTransform()
{
    glm::mat4 transform(1.0);

    transform = glm::translate(transform, m_Position);
    transform = glm::rotate(transform, 45.0f, m_Rotation);
    transform = glm::scale(transform, m_Scale);

    return transform;
}
