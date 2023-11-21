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
        texture->CreateTexture(1024, 1024);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);

        auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
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
        ImGui::Begin("Postprocess");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);
        bool effect = params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect))
        {
            if (effect) params |= INVERT_MASK;
            else params ^= INVERT_MASK;
        }
        effect = params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            if (effect) params |= GRAYSCALE_MASK;
            else params ^= GRAYSCALE_MASK;
        }
        effect = params & TINT_MASK;
        if (ImGui::Checkbox("Tint", &effect))
        {
            if (effect)
            {
                params |= TINT_MASK;
            }
            else params ^= TINT_MASK;
        }
        ImGui::ColorEdit3("Tint Color", glm::value_ptr(tintColor));
        effect = params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            if (effect) params |= GRAIN_MASK;
            else params ^= GRAIN_MASK;
        }
        ImGui::DragFloat("Grain Amount", &grainAmount, 0.01f);
        effect = params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            if (effect) params |= SCANLINE_MASK;
            else params ^= SCANLINE_MASK;
        }
        ImGui::DragFloat("Scanline Intensity", &scanlineIntensity, 0.01f);
        effect = params & CUSTOM_MASK;
        if (ImGui::Checkbox("Custom", &effect))
        {
            if (effect) params |= CUSTOM_MASK;
            else params ^= CUSTOM_MASK;
        }
        ImGui::DragFloat("Kernel", &kernel, 0.1f);
        ImGui::DragFloat("Texel Size X", &texelSizeX, 0.1f);
        ImGui::DragFloat("Texel Size Y", &texelSizeY, 0.1f);
        ImGui::End();

        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", params);
            program->SetUniform("tintColor", tintColor);
            program->SetUniform("grainAmount", grainAmount);
            program->SetUniform("scanlineIntensity", scanlineIntensity);
            program->SetUniform("kernel", kernel);
            program->SetUniform("texelSizeX", texelSizeX);
            program->SetUniform("texelSizeY", texelSizeY);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewPort(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 1, 1, 1 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewPort();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);
        
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();

    }
}
