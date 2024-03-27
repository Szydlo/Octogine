#pragma once

#include "core/Window.h"
#include "core/Events.h"
#include "core/Input.h"

#include "graphics/Shader.h"
#include "graphics/VertexArray.h"
#include "graphics/Buffer.h"
#include "graphics/Renderer.h"
#include "graphics/Camera.h"
#include "graphics/Texture2D.h"
#include "graphics/Mesh.h"
#include "graphics/Model.h"

#include "lighting/DirectionalLight.h"

/* 
    API CHEATSHEET

    v0.1 - BASIC STUFF
    Octogine::Core - initializes all basic systems, like opengl. - DONE
    Octogine::Window - creates window - DONE
    Octogine::Camera - creates basic view - DONE
    Octogine::Input - manages keys, and cursor, with Event::OnKeyPressed etc - DONE
    Octogine::Shader - creates shader, with ability to set vec3, floats, etc. - DONE
    Octogine::Mesh - takes array of vertices and indicies, and puts it into gpu (VAO, VBO, EBO).  - DONE
    Octogine::Texture2D - puts a image to gpu - DONE
    Octogine::Renderer base for renderer - DONE

    To finish 0.1 draw basic cube with texture, and rotate camera arround it

    v0.2 - MODEL IMPORTING
    Octogine::Model - uses Octogine::Mesh, Shader, Texture2D to load model from file (pref gltf), and creates full ready model - DONE

    To finish v0.2 import model from blender

    v0.3 - BASIC LIGHTING
    Octogine::Shadow
    Octogine::DirectLight
    Octogine::SpotLight
    Octogine::PointLight
    Octogine::SkyBox

    v0.4 - ADVANCED LIGHTING
    Octogine::HDR
    Octogine::Normal
    Octogine::Parallax
    Octogine::SSAO
    Octogine::Bloom

    v0.5 - MATERIALS
    Octogine::Material - ability to create a PBR material.

    v0.6 - PHYSICS
    Implement jolt physics engine
    Octogine::RigidBody
    Octogine::StaticBody
    Octogine::KinematicBody
    Octogine::RayCast
    Octogine::Collider

    v0.7 - ANIMATIONS
    v0.8 - AUDIO SYSTEM - STEAM AUDIO?
    v0.9 - ASSET MANAGER
    v1.0 - SCENE SYSTEM AND CHILDS - GODOT LIKE SYSTEM?

    v1.1 - FIXING BUGS, BASIC GAME
    v1.2 - SCRIPTING LANGUAGE
    v1.2 - TOOLS LIKE EDITOR
    v1.3 - NETWORKING
    v1.4 - Vehicles, softbodies
*/