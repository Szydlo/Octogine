#include "Shape.h"

using namespace Octo;

BoxShape::BoxShape(const glm::vec3 size)
{
    const JPH::BoxShapeSettings boxSettings(JoltUtils::convertGlmVec3(size));

    m_ShapeResult = boxSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

CapsuleShape::CapsuleShape(const float halfHeight, const float radius)
{
    const JPH::CapsuleShapeSettings capsuleSettings(halfHeight, radius);

    m_ShapeResult = capsuleSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

CylinderShape::CylinderShape(const float halfHeight, const float radius)
{
    const JPH::CylinderShapeSettings cylinderSettings(halfHeight, radius);

    m_ShapeResult = cylinderSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

SphereShape::SphereShape(const float radius)
{
    const JPH::SphereShapeSettings sphereSettings(radius);

    m_ShapeResult = sphereSettings.Create();
    m_Shape = m_ShapeResult.Get();
}

TriangleShape::TriangleShape(const glm::vec3 point1, const glm::vec3 point2, const glm::vec3 point3)
{
    const JPH::TriangleShapeSettings triangleSettings(JoltUtils::convertGlmVec3(point1), JoltUtils::convertGlmVec3(point2), JoltUtils::convertGlmVec3(point3));

    m_ShapeResult = triangleSettings.Create();
    m_Shape = m_ShapeResult.Get();
}