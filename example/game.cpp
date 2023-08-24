#include "game.hpp"
#include "cpengine/debug/debug.hpp"
#include "cpengine/modules/input/input.hpp"
#include "cpengine/modules/resources/resources.hpp"
#include "cpengine/modules/graphics/shader.hpp"
#include "cpengine/modules/graphics/texture2D.hpp"

#include "cpengine/modules/world/world.hpp"
#include "cpengine/components/transform.hpp"

#include "cpengine/definitions/stringify.hpp"

Game::Game()
{}

Game::~Game()
{
    
}

void Game::OnEngineStart()
{
    GetModules()->resources->Load<CPGFramework::Graphics::Shader>("shaders", "simple");
    GetModules()->resources->Load<CPGFramework::Graphics::Texture2D>("textures", "tex.jpg");
    auto* t0 = entity.AddComponent<CPGFramework::Components::Transform>();
    auto* t1 = entity2.AddComponent<CPGFramework::Components::Transform>();

    t1->SetParent(*t0);
}

void Game::OnEngineQuit()
{

}

void Game::Update() 
{
    if(GetModules()->input->IsKeyPressed(Key::A))
    {
        auto* child = entity2.GetComponent<CPGFramework::Components::Transform>();
        auto* owner = child->GetParent();
        auto* root = owner->GetParent();
        
        child->Translate(VEC3(0,1,0), 2.0f);
        // owner->Translate(VEC3(0,1,0), 1.0f);

        DEBUG_LOG("CHILD (WORLD)>>", ToString(child->GetTransformMatrix()));
        DEBUG_LOG("OWNER (WORLD)>>", ToString(owner->GetTransformMatrix()));
        DEBUG_LOG("CHILD (LOCAL)>>", ToString(child->WorldToLocalTransformMatrix()));
        DEBUG_LOG("OWNER (LOCAL)>>", ToString(owner->WorldToLocalTransformMatrix()));
    }

        if(GetModules()->input->IsKeyPressed(Key::D))
    {
        auto* child = entity2.GetComponent<CPGFramework::Components::Transform>();
        auto* owner = child->GetParent();
        auto* root = owner->GetParent();
        
        // child->Translate(VEC3(0,1,0), 2.0f);
        owner->Translate(VEC3(0,1,0), -1.0f);

        DEBUG_LOG("CHILD (WORLD)>>", ToString(child->GetTransformMatrix()));
        DEBUG_LOG("OWNER (WORLD)>>", ToString(owner->GetTransformMatrix()));
        DEBUG_LOG("CHILD (LOCAL)>>", ToString(child->WorldToLocalTransformMatrix()));
        DEBUG_LOG("OWNER (LOCAL)>>", ToString(owner->WorldToLocalTransformMatrix()));
    }
}
void Game::FixedUpdate() {}
void Game::LateUpdate() {}