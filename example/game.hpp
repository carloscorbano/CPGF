#pragma once

#include "cpengine/engine.hpp"
#include "cpengine/modules/world/entity.hpp"

class Game : public CPGFramework::Engine
{
public:
    Game();
    ~Game();

    void OnEngineStart();
    void OnEngineQuit();
    void Update();
    void FixedUpdate();
    void LateUpdate();

    CPGFramework::World::Entity entity;
    CPGFramework::World::Entity entity2;
};