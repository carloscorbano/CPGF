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
    Components::Transform* entityTransform = entity.GetComponent<Components::Transform>();
    const DOUBLE deltaTime = GetModules()->gametime->GetDeltaTime();

    if(!entityTransform) return;

    if(GetModules()->input->IsKeyPressed(Key::T))
    {
        curSpace = curSpace == Space::LOCAL ? Space::WORLD : Space::LOCAL;
        DEBUG_LOG("CHANGED CUR SPACE TO", curSpace == Space::LOCAL ? "LOCAL" : "WORLD");
    }

    if(GetModules()->input->IsKeyHolded(Key::A))
    {
        entityTransform->
            Translate(VEC3(-1,0,0),
            2.0f * deltaTime, 
            curSpace);
    }
    else if(GetModules()->input->IsKeyHolded(Key::D))
    {
        entityTransform->
            Translate(VEC3(1,0,0),
            2.0f * deltaTime,
            curSpace);
    }
    
    if(GetModules()->input->IsKeyHolded(Key::W))
    {
        // entityTransform->
        //     Translate(VEC3(0,1,0),
        //     2.0f * deltaTime,
        //     curSpace);
        entityTransform->Translate(entityTransform->GetUpVector(), 2.0f * deltaTime);
    }
    else if(GetModules()->input->IsKeyHolded(Key::S))
    {
        // entityTransform->
        //     Translate(VEC3(0,-1,0),
        //     2.0f * deltaTime,
        //     curSpace);

        entityTransform->Translate(entityTransform->GetUpVector(), -2.0f * deltaTime);
    }

    if(GetModules()->input->IsKeyHolded(Key::X))
    {
        entityTransform->
            Translate(VEC3_ZERO);
    }

    // if(GetModules()->input->IsKeyHolded(Key::Q))
    // {
    //     entityTransform->
    //         Translate(VEC3(0,0,1),
    //         2.0f * deltaTime);
    // }

    // if(GetModules()->input->IsKeyHolded(Key::E))
    // {
    //     entityTransform->
    //         Translate(VEC3(0,0,-1),
    //         2.0f * deltaTime);
    // }

    if(GetModules()->input->IsKeyHolded(Key::K))
    {
        entityTransform->Rotate(VEC3(0,1,0), 
                120.0f * deltaTime);
        // DEBUG_LOG("ROTATION", ToString(RadToDeg(entityTransform->GetEulerRotation())));
        
    }

    if(GetModules()->input->IsKeyHolded(Key::L))
    {
        entityTransform->Rotate(VEC3(0,1,0), 
                -120.0f * deltaTime);
        DEBUG_LOG("ROTATION", ToString(entityTransform->GetEulerRotation()));
    }

    if(GetModules()->input->IsKeyPressed(Key::Y))
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