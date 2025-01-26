#include "Shape.h"

using namespace Octo;

BoxShape::BoxShape(glm::vec3 size)
{
    JPH::BoxShapeSettings boxSettings(JoltUtils::convertGlmVec3(size));

    m_ShapeResult = boxSettings.Create();
    m_Shape = m_ShapeResult.Get();
}



CapsuleShape::CapsuleShape(float halfHeight, float radius)
{
    JPH::CapsuleShapeSettings capsuleSettings(halfHeight, radius);

    m_ShapeResult = capsuleSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

CylinderShape::CylinderShape(float halfHeight, float radius)
{
    JPH::CylinderShapeSettings cylinderSettings(halfHeight, radius);

    m_ShapeResult = cylinderSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

SphereShape::SphereShape(float radius)
{
    JPH::SphereShapeSettings sphereSettings(radius);

    m_ShapeResult = sphereSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

TriangleShape::TriangleShape(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
    JPH::TriangleShapeSettings triangleSettings(JoltUtils::convertGlmVec3(point1), JoltUtils::convertGlmVec3(point2), JoltUtils::convertGlmVec3(point3));

    m_ShapeResult = triangleSettings.Create();
    m_Shape = m_ShapeResult.Get();
}