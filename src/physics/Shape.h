#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/CapsuleShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/TriangleShape.h>

#include <glm/glm.hpp>

#include "JoltUtils.h"

namespace Octo
{
    struct Shape
    {
        public:
            JPH::Shape* geJoltShape() { return m_Shape; }
            JPH::ShapeSettings::ShapeResult getShapeSettingsResult() { return m_ShapeResult; }
        protected:
            JPH::Shape* m_Shape;
            JPH::ShapeSettings::ShapeResult m_ShapeResult;
    };

    struct BoxShape : public Shape 
    {
        BoxShape(glm::vec3 size);
    };

    struct CapsuleShape : public Shape 
    {
        CapsuleShape(float halfHeight, float radius);
    };

    struct CylinderShape : public Shape 
    {
        CylinderShape(float halfHeight, float radius);
    };

    struct SphereShape : public Shape 
    {
        SphereShape(float radius);
    };

    struct TriangleShape : public Shape 
    {
        TriangleShape(glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
    };
}