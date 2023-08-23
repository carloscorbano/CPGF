#include "game.hpp"
#include "cpengine/debug/debug.hpp"
#include "cpengine/modules/input/input.hpp"
#include "cpengine/modules/resources/resources.hpp"
#include "cpengine/modules/graphics/shader.hpp"
#include "cpengine/modules/graphics/texture2D.hpp"

#include "cpengine/modules/world/world.hpp"
#include "cpengine/modules/world/entity.hpp"
#include "cpengine/components/transform.hpp"

Game::Game()
{}

Game::~Game()
{
    
}

void Game::OnEngineStart()
{
    GetModules()->resources->Load<CPGFramework::Graphics::Shader>("shaders", "simple");
    GetModules()->resources->Load<CPGFramework::Graphics::Texture2D>("textures", "tex.jpg");
    CPGFramework::World::Entity entity;
    entity.AddComponent<CPGFramework::Components::Transform>();
}

void Game::OnEngineQuit()
{

}

void Game::Update() 
{
}
void Game::FixedUpdate() {}
void Game::LateUpdate() {}