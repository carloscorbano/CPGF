#include "game.hpp"
#include "cpengine/debug/debug.hpp"
#include "cpengine/modules/input/input.hpp"
#include "cpengine/modules/resources/resources.hpp"
#include "cpengine/modules/graphics/shader.hpp"
#include "cpengine/modules/graphics/texture2D.hpp"

Game::Game()
{}

Game::~Game()
{
    
}

void Game::OnEngineStart()
{
    GetModules()->resources->Load<CPGFramework::Graphics::Shader>("shaders", "simple");
    GetModules()->resources->Load<CPGFramework::Graphics::Texture2D>("textures", "tex.jpg");
}

void Game::OnEngineQuit()
{

}

void Game::Update() 
{
}
void Game::FixedUpdate() {}
void Game::LateUpdate() {}