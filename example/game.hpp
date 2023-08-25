#pragma once

#include "cpengine/engine.hpp"
#include "cpengine/modules/world/entity.hpp"

#include "cpengine/modules/graphics/vertex_array.hpp"
#include "cpengine/modules/graphics/index_buffer.hpp"
#include "cpengine/modules/graphics/material.hpp"

using namespace CPGFramework;

class Game : public Engine
{
public:
    Game();
    ~Game();

    void OnEngineStart();
    void OnEngineQuit();
    void Update();
    void FixedUpdate();
    void LateUpdate();
    void Draw();

    World::Entity entity;
    World::Entity entity2;

    Graphics::VertexArray* VA;
    Graphics::VertexBuffer* VB;
    Graphics::IndexBuffer* IB;
    Graphics::Material* material;
};