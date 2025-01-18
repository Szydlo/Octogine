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
#include "graphics/FrameBuffer.h"

#include "lighting/DirectionalLight.h"
#include "lighting/PointLight.h"
#include "lighting/SpotLight.h"
#include "lighting/Material.h"
#include "lighting/SkyBox.h"
#include "lighting/Shadow.h"
#include "lighting/LightingManager.h"

#include "physics/RigidBody.h"
#include "physics/Shape.h"
#include "physics/Collider.h"
#include "physics/Layers.h"
#include "physics/RayCast.h"
#include "physics/Physics.h"

#include "audio/Audio.h"
#include "audio/Sound.h"
#include "audio/AudioListener.h"

#include "models/Model.h"

#include "animations/Animation.h"
#include "animations/Animator.h"

#include "assetmanager/AssetManager.h"

#include "scenes/Scene.h"
#include "scenes/3D/Node3D.h"
#include "scenes/3D/Model3D.h"
#include "scenes/3D/Sun3D.h"
#include "scenes/3D/Spotlight3D.h"
#include "scenes/3D/Pointlight3D.h"

/* 
    API CHEATSHEET

    v0.1 - BASIC STUFF - DONE
    Octogine::Core - initializes all basic systems, like opengl. - DONE
    Octogine::Window - creates window - DONE
    Octogine::Camera - creates basic view - DONE
    Octogine::Input - manages keys, and cursor, with Event::OnKeyPressed etc - DONE
    Octogine::Shader - creates shader, with ability to set vec3, floats, etc. - DONE
    Octogine::Mesh - takes array of vertices and indicies, and puts it into gpu (VAO, VBO, EBO).  - DONE
    Octogine::Texture2D - puts a image to gpu - DONE
    Octogine::Renderer base for renderer - DONE

    To finish 0.1 draw basic cube with texture, and rotate camera arround it

    v0.2 - MODEL IMPORTING - DONE
    Octogine::Model - uses Octogine::Mesh, Shader, Texture2D to load model from file (pref gltf), and creates full ready model - DONE

    To finish v0.2 import model from blender

    v0.3 - BASIC LIGHTING - DONE
    Octogine::Shadow - DONE
    Octogine::DirectLight - DONE
    Octogine::SpotLight - DONE
    Octogine::PointLight - DONE
    Octogine::SkyBox - DONE

    v0.4 - PHYSICS - DONE
    Implement jolt physics engine - DONE
    Octogine::RigidBody - DONE
    Octogine::StaticBody - DONE
    Octogine::KinematicBody - DONE
    Octogine::RayCast  - DONE
    Octogine::Collider - DONE
    Octogine::Shape - DONE

    v0.5 - AUDIO SYSTEM - DONE
    Loading audio files - DONE
    Octogine::Audio - initializing audio system - DONE
    Octogine::AudioListener - lister of audio - DONE
    Octogine::Sound playing sound/music - DONE

    v0.6 - ANIMATIONS - DONE
    Octogine::Animation - contains keys data of animation - done
    Ocotogine::SkeletalMesh + Model - contains regular mesh + bones and positions - done
    Octogine::Animator - (blends between animations and plays them with skeletal mesh) - done

    v0.7 - ASSET MANAGER - DONE

    v0.8 - SCENE SYSTEM  - DONE

    v0.9 & 1.0 - RENDERER & MATERIALS
    Implement shadows and lights with node system.
    point lights
    spot lights
    lighting system
    Octogine::Normals
    Octogine::PBR
    Octogine::IBL
    Octogine::SSAO
    Octogine::Bloom

    To finish v1.0 create scene with multiple point lights, materials, shiny and plastic,
    with reflection, like mirror, and different objects and shadows.

    projects:
        - basic model viewer
        - basic game


    v1.1 - TOOLS LIKE EDITOR
    v1.2 - FIXING BUGS, BASIC GAME
    v1.3 - SCRIPTING LANGUAGE
    v1.4 - Vehicles, softbodies
    v1.5 - NETWORKING
*/