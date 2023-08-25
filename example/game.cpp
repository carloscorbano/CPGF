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
    float vertices[] = 
    {
        //position                 //color                 //texCoord
        -0.5f,  -0.5f, 0.0f,       1.0f, 1.0f, 1.0f,       0.0f, 0.0f, //0
         0.5f,  -0.5f, 0.0f,       1.0f, 1.0f, 1.0f,       1.0f, 0.0f, //1
        -0.5f,   0.5f, 0.0f,       1.0f, 1.0f, 1.0f,       0.0f, 1.0f, //2
         0.5f,   0.5f, 0.0f,       1.0f, 1.0f, 1.0f,       1.0f, 1.0f  //3
    };

    GLuint indices[] = 
    {
        0, 2, 1,
        1, 2, 3
    };

    material = new Graphics::Material();
    material->SetShader(GetModules()->resources->Load<Graphics::Shader>("shaders", "simple"));
    VA = new Graphics::VertexArray();
    VB = new Graphics::VertexBuffer(vertices, sizeof(vertices));
    IB = new Graphics::IndexBuffer(indices, 6);

    Graphics::VertexBufferLayout vbl;
    vbl.Push<GLfloat>(3);
    vbl.Push<GLfloat>(3);
    vbl.Push<GLfloat>(2);

    VA->AddBuffer(*VB, vbl);

    Graphics::TextureData td(GetModules()->resources->Load<Graphics::Texture2D>("textures", "tex.jpg"));
    material->Push<GLint>("outTexture", 0);
    material->Push<Graphics::TextureData>(td.texture->GetFilename(), td);

    auto* t0 = entity.AddComponent<Components::Transform>();
    auto* t1 = entity2.AddComponent<Components::Transform>();

    t1->SetParent(*t0);
    t1->Translate(VEC3(2, 0, 0));
}

void Game::OnEngineQuit()
{
    delete VA;
    delete VB;
    delete IB;
    delete material;
}

void Game::Update() 
{
    if(GetModules()->input->IsKeyHolded(Key::A))
    {
        // auto* child = entity2.GetComponent<Components::Transform>();
        // auto* owner = child->GetParent();
        // auto* root = owner->GetParent();
        
        // //child->Translate(VEC3(0,1,0), 2.0f);
        // owner->Scale(VEC3(1,2,1));

        // DEBUG_LOG("CHILD (WORLD)>>", ToString(child->GetTransformMatrix()));
        // DEBUG_LOG("OWNER (WORLD)>>", ToString(owner->GetTransformMatrix()));
        // DEBUG_LOG("CHILD (LOCAL)>>", ToString(child->WorldToLocalTransformMatrix()));
        // DEBUG_LOG("OWNER (LOCAL)>>", ToString(owner->WorldToLocalTransformMatrix()));
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
        // auto* child = entity2.GetComponent<Components::Transform>();
        // auto* owner = child->GetParent();
        // auto* root = owner->GetParent();
        
        // //child->Translate(VEC3(0,1,0), 2.0f);
        // owner->Scale(VEC3(1,2,1));

        // DEBUG_LOG("CHILD (WORLD)>>", ToString(child->GetTransformMatrix()));
        // DEBUG_LOG("OWNER (WORLD)>>", ToString(owner->GetTransformMatrix()));
        // DEBUG_LOG("CHILD (LOCAL)>>", ToString(child->WorldToLocalTransformMatrix()));
        // DEBUG_LOG("OWNER (LOCAL)>>", ToString(owner->WorldToLocalTransformMatrix()));
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
        entity.GetComponent<Components::Transform>()->Scale(VEC3(2,2,2));
    }

    if(GetModules()->input->IsKeyHolded(Key::E))
    {
        entity.GetComponent<Components::Transform>()->Scale(VEC3(1,1,1));
    }

}
void Game::FixedUpdate() {}
void Game::LateUpdate() {}

void Game::Draw()
{
    if(material->GetShader()->GetResourceState() == Resources::ResourceState::OK)
    {
        material->Bind();
        VA->Bind();
        IB->Bind();

        glEnable(GL_DEPTH_TEST);
        //glEnable(GL_CULL_FACE);

        MAT4 model, view, projection;
        model = view = projection = MAT4_IDENTITY;

        view = glm::lookAt(VEC3(0,0,5.0f), VEC3(0,0,0), VEC3(0,1,0));
        projection = glm::perspective(glm::radians(45.0f), (1320.0f / 780.0f), 0.1f, 1000.0f);
        material->GetShader()->SetUniform<MAT4>("v", view);
        material->GetShader()->SetUniform<MAT4>("p", projection);

        model = entity.GetComponent<Components::Transform>()->GetTransformMatrix();
        material->GetShader()->SetUniform<MAT4>("m", model);
        
        glDrawElements(GL_TRIANGLES, IB->GetCount(), GL_UNSIGNED_INT, 0);

        model = entity2.GetComponent<Components::Transform>()->GetTransformMatrix();
        material->GetShader()->SetUniform<MAT4>("m", model);

        glDrawElements(GL_TRIANGLES, IB->GetCount(), GL_UNSIGNED_INT, 0);
    }
}