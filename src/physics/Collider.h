#pragma once

#include <Jolt/Jolt.h>

#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>

#include <glm/glm.hpp>

#include "JoltUtils.h"
#include "Shape.h"

namespace Octo
{
    class Collider
    {
        public:
            explicit Collider(Shape& shape);

            [[nodiscard]] Shape& getShape() const { return m_Shape; }
        private:
            Shape& m_Shape;
    };
}