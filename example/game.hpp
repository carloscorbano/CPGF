#pragma once

#include "cpengine/engine.hpp"

class Game : public CPGFramework::Engine
{
public:
    Game();
    ~Game();

    void Update();
    void FixedUpdate();
    void LateUpdate();
};