#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"
#include "Core/StringUtils.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace lady
{
    bool World07::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_editor = std::make_unique<Editor>();
        m_scene->Load("scenes/scene_editor.json");
        m_scene->Load("scenes/sceneshadow.json");
        m_scene->Initialize();


        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthbuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);

        //auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");

        /*auto material = GET_RESOURCE(Material, "materials/postprocess.mtrl");
        if (material)
        {
            material->albedoTexture = texture;
        }*/

        //material->albedoTexture = texture;

        auto materials = GET_RESOURCES(Material);
        for (auto material : materials)
        {
            material->depthTexture = texture;
        }


        return true;
    }

    void World07::Shutdown()
    {
    }

    void World07::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);

        m_editor->Update();
        m_editor->ProcessGui(m_scene.get());

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {
        /*
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewPort(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame(glm::vec3{ 1, 0, 0 });
        m_scene->Draw(renderer);

        framebuffer->Unbind();
        */

        // *** PASS 2 ***
        //renderer.ResetViewPort();
        //renderer.BeginFrame();
        //m_scene->Draw(renderer);
        //
        //// post-render
        //ENGINE.GetSystem<Gui>()->Draw();
        //renderer.EndFrame();

        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.SetViewPort(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();
        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto light : lights)
        {
            if(light->castShadow)
            {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelRenderComponent>();
        for (auto models : models)
        {
            if (models->castShadow)
            {
                program->SetUniform("models", models->m_owner->transform.GetMatrix());
                models->m_model->Draw();
            }
        }

        framebuffer->Unbind();

        renderer.ResetViewPort();
        renderer.BeginFrame();
        m_scene->Draw(renderer);

        ENGINE.GetSystem<Gui>()->Draw();

        renderer.EndFrame();


    }
}
