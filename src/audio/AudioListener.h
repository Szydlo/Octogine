#pragma once

#include <glm/glm.hpp>

#include "../../vendor/miniaudio/miniaudio.h"

namespace Octo
{
    struct AudioListener
    {
        glm::vec3 position = glm::vec3(0);
        glm::vec3 direction = glm::vec3(0);
        glm::vec3 worldUp = glm::vec3(0);

        ma_engine* engine = nullptr;
        unsigned int listener;

        void setListener(ma_engine* _engine, unsigned int _listener)
        {
            engine = _engine;
            listener = _listener;

            ma_engine_listener_set_enabled(_engine, _listener, MA_TRUE);
        }

        void setData()
        {
            if (engine == nullptr) return;

            ma_engine_listener_set_position(engine, listener, position.x, position.y, position.z);
            ma_engine_listener_set_direction(engine, listener, direction.x, direction.y, direction.z);
            ma_engine_listener_set_world_up(engine, listener, worldUp.x, worldUp.y, worldUp.z);
        }
    };
}