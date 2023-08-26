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

    t1->SetParent(*t0);
    t1->Translate(VEC3(2, 0, 0));
}

void Game::OnEngineQuit()
{
    if(mesh) delete mesh;
}

void Game::Update() 
{
    if(GetModules()->input->IsKeyHolded(Key::A))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(-1,0,0),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }
    else if(GetModules()->input->IsKeyHolded(Key::D))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(1,0,0),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }

    if(GetModules()->input->IsKeyHolded(Key::W))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(0,1,0),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }
    else if(GetModules()->input->IsKeyHolded(Key::S))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(0,-1,0),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }

    if(GetModules()->input->IsKeyHolded(Key::Q))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(0,0,1),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }

    if(GetModules()->input->IsKeyHolded(Key::E))
    {
        entity.GetComponent<Components::Transform>()->
            Translate(VEC3(0,0,-1),
            2.0f * GetModules()->gametime->GetDeltaTime());
    }

        if(GetModules()->input->IsKeyHolded(Key::K))
    {
        entity.GetComponent<Components::Transform>()->Rotate(VEC3(0,1,0), 
                120.0f * GetModules()->gametime->GetDeltaTime());
    }

    if(GetModules()->input->IsKeyHolded(Key::L))
    {
        entity.GetComponent<Components::Transform>()->Rotate(VEC3(0,1,0), 
                -120.0f * GetModules()->gametime->GetDeltaTime());
    }

    if(GetModules()->input->IsKeyPressed(Key::K))
    {
        auto* transform = entity2.GetComponent<Components::Transform>();
        if(transform->GetParent())
        {
            transform->RemoveParent();
        }
        else
        {
            transform->SetParent(*entity.GetComponent<Components::Transform>());
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
        
        model = entity.GetComponent<Components::Transform>()->GetTransformMatrix();
        shader->SetUniform<MAT4>("model", model);
        mesh->Draw(true);

        model = entity2.GetComponent<Components::Transform>()->GetTransformMatrix();
        shader->SetUniform<MAT4>("model", model);
        mesh->Draw();
    }
}