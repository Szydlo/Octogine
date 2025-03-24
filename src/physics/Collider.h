#pragma once

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