#pragma once

#include "cpengine/engine.hpp"
#include "cpengine/modules/world/entity.hpp"
#include "cpengine/definitions/space.hpp"
#include "cpengine/definitions/constants.hpp"
#include "cpengine/modules/graphics/mesh.hpp"

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

    Graphics::Mesh* mesh;
    Space curSpace = Space::LOCAL;
    FLOAT scale = 1.0f;
};