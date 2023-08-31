#include "game.hpp"
#include "cpengine/debug/debug.hpp"
#include "cpengine/modules/input/input.hpp"
#include "cpengine/modules/resources/resources.hpp"
#include "cpengine/modules/graphics/shader.hpp"
#include "cpengine/modules/graphics/texture2D.hpp"
#include "cpengine/modules/time/game_time.hpp"
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
    Graphics::TextureData td(GetModules()->resources->Load<Graphics::Texture2D>("textures", "tex.jpg"));

    mesh = new Graphics::Mesh();

    Graphics::Material& mat = mesh->GetMaterial();

    mat.SetShader(GetModules()->resources->Load<Graphics::Shader>("shaders", "simple"));
    mat.Push<GLint>("ourTexture", 0);
    mat.Push<Graphics::TextureData>(td.texture->GetFilename(), td);

    mesh->AddQuad(  Graphics::Vertex({ VEC3(-0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );

    mesh->AddQuad(  Graphics::Vertex({ VEC3(0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );

    mesh->AddQuad(  Graphics::Vertex({ VEC3(0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );

    mesh->AddQuad(  Graphics::Vertex({ VEC3(-0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );

    mesh->AddQuad(  Graphics::Vertex({ VEC3(-0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, 0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );

    mesh->AddQuad(  Graphics::Vertex({ VEC3(-0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(-0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 0.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, -0.5f, 0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(0.0f, 1.0f) }), 
                    Graphics::Vertex({ VEC3(0.5f, -0.5f, -0.5f), VEC3(1.0f, 1.0f, 1.0f), VEC2(1.0f, 1.0f) })
                );
    
    mesh->Compile();

    auto* t0 = entity.AddComponent<Components::Transform>();
    auto* t1 = entity2.AddComponent<Components::Transform>();

    t1->SetParent(t0);
    t1->Translate(VEC3(2, 0, 0));
}

void Game::OnEngineQuit()
{
    if(mesh) delete mesh;
}

void Game::Update() 
{
    Components::Transform* entityTransform = entity.GetComponent<Components::Transform>();
    const DOUBLE deltaTime = GetModules()->gametime->GetDeltaTime();

    if(!entityTransform) return;

    if(GetModules()->input->IsKeyPressed(Key::T))
    {
        curSpace = curSpace == Space::LOCAL ? Space::WORLD : Space::LOCAL;
        DEBUG_LOG("CHANGED CUR SPACE TO", curSpace == Space::LOCAL ? "LOCAL" : "WORLD");
    }

    if(GetModules()->input->IsKeyHolded(Key::W))
    {
        entityTransform->Translate(VEC3_UP, 2.0f * deltaTime, curSpace);
    }
    else if(GetModules()->input->IsKeyHolded(Key::S))
    {
        entityTransform->Translate(VEC3_UP, -2.0f * deltaTime, curSpace);
    }

    if(GetModules()->input->IsKeyHolded(Key::A))
    {
        entityTransform->Translate(VEC3_RIGHT, -2.0f * deltaTime, curSpace);
    }
    else if(GetModules()->input->IsKeyHolded(Key::D))
    {
        entityTransform->Translate(VEC3_RIGHT, 2.0f * deltaTime, curSpace);
    }

    if(GetModules()->input->IsKeyHolded(Key::ARROW_UP))
    {
        entityTransform->Rotate(VEC3_RIGHT, 120.0f * deltaTime, curSpace);
    }
    else if(GetModules()->input->IsKeyHolded(Key::ARROW_DOWN))
    {
        entityTransform->Rotate(VEC3_RIGHT, -120.0f * deltaTime, curSpace);
    }

    if(GetModules()->input->IsKeyHolded(Key::ARROW_RIGHT))
    {
        entityTransform->Rotate(VEC3_UP, 120.0f * deltaTime, curSpace);
    }
    else if(GetModules()->input->IsKeyHolded(Key::ARROW_LEFT))
    {
        entityTransform->Rotate(VEC3_UP, -120.0f * deltaTime, curSpace);
    }


    if(GetModules()->input->IsKeyPressed(Key::X))
    {
        entityTransform->Rotate(VEC3_RIGHT, 45.0f);
        entityTransform->Rotate(VEC3_UP, 45.0f);
    }

    if(GetModules()->input->IsKeyPressed(Key::U))
    {
        scale += 0.1f;
        DEBUG_LOG(scale);
        entityTransform->Scale(VEC3(scale, scale, scale));

    }
    else if(GetModules()->input->IsKeyPressed(Key::J))
    {
        scale -= 0.1f;
        DEBUG_LOG(scale);
        entityTransform->Scale(VEC3(scale, scale, scale));
    }

    Components::Transform* entity2Transform = entity2.GetComponent<Components::Transform>();
    if(!entity2Transform) return;

    if(GetModules()->input->IsKeyHolded(Key::Y))
    {
        entity2Transform->Translate(entity2Transform->GetForwardVector(), 2.0f * deltaTime);

    }
    else if(GetModules()->input->IsKeyHolded(Key::H))
    {
        entity2Transform->Translate(entity2Transform->GetForwardVector(), -2.0f * deltaTime);
    }

    if(GetModules()->input->IsKeyPressed(Key::O))
    {
        if(entity2Transform->GetParent())
        {
            entity2Transform->SetParent(nullptr);
            DEBUG_LOG("CLEARED PARENT");
        }
        else
        {
            entity2Transform->SetParent(entityTransform);
            DEBUG_LOG("SETTED PARENT");
        }
    }
}
void Game::FixedUpdate() {}
void Game::LateUpdate() {}

void Game::Draw()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if(mesh)
    {
        mesh->Bind();

        MAT4 model, view, projection;
        model = view = projection = MAT4_IDENTITY;
        view = glm::lookAt(VEC3(0,0,5.0f), VEC3(0,0,0), VEC3(0,1,0));
        projection = glm::perspective(glm::radians(45.0f), (1320.0f / 780.0f), 0.1f, 1000.0f);

        Graphics::Shader* shader = mesh->GetMaterial().GetShader();
        shader->SetUniform<MAT4>("view", view);
        shader->SetUniform<MAT4>("projection", projection);
        
        model = entity.GetComponent<Components::Transform>()->GetWorldMatrix();
        shader->SetUniform<MAT4>("model", model);
        mesh->Draw(true);

        model = entity2.GetComponent<Components::Transform>()->GetWorldMatrix();
        shader->SetUniform<MAT4>("model", model);
        mesh->Draw();
    }
}