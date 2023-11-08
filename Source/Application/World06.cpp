#include "World06.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"
#include "Core/StringUtils.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace lady
{
    bool World06::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scenefb.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }
        

        {
            auto actor = CREATE_CLASS(Actor);
            actor->name = "camera1";
            actor->transform.position = glm::vec3{ 0, 0, 3 };
            actor->transform.rotation = glm::radians(glm::vec3{ 0, 180, 0 });

            auto cameraComponent = CREATE_CLASS(CameraComponent);
            cameraComponent->SetPerspective(70.0f, ENGINE.GetSystem<Renderer>()->GetWidth() / (float)ENGINE.GetSystem<Renderer>()->GetHeight(), 0.1f, 100.0f);
            actor->AddComponent(std::move(cameraComponent));

            auto cameraController = CREATE_CLASS(CameraController);
            cameraController->speed = 2;
            cameraController->sensitivity = 2;
            cameraController->m_owner = actor.get();
            cameraController->Initialize();
            actor->AddComponent(std::move(cameraController));

            m_scene->Add(std::move(actor));
        }

        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();
        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("cube")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewPort(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 1, 0, 0 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("cube")->active = true;

        renderer.ResetViewPort();
        renderer.BeginFrame();
        m_scene->Draw(renderer);
        
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();

    }
}
