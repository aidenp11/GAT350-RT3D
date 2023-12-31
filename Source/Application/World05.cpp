#include "World05.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"
#include "Core/StringUtils.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

#define INTERLEAVE

namespace lady
{
    bool World05::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene.json");
        m_scene->Initialize();

        /*{
            auto actor = CREATE_CLASS(Actor);
            actor->name = "actor1";
            actor->transform.position = glm::vec3{ 0, 0, 0 };
            auto modelComponent = CREATE_CLASS(ModelRenderComponent);
            modelComponent->m_model = std::make_shared<Model>();
            modelComponent->m_model->SetMaterial(GET_RESOURCE(Material, "materials/ogre.mtrl"));
            modelComponent->m_model->Load("models/ogre.obj", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });
            actor->AddComponent(std::move(modelComponent));
            m_scene->Add(std::move(actor));
        }*/

        //{
        //    auto actor = CREATE_CLASS(Actor);
        //    actor->name = "light1";
        //    actor->transform.position = glm::vec3{ 3, 3, 3 };
        //    auto lightComponent = CREATE_CLASS(LightComponent);
        //    lightComponent->type = LightComponent::eType::Point;
        //    lightComponent->color = glm::vec3{ 1 };//glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });
        //    lightComponent->intensity = 1;
        //    lightComponent->range = 20;
        //    lightComponent->innerAngle = 10.0f;
        //    lightComponent->outerAngle = 30.0f;
        //    actor->AddComponent(std::move(lightComponent));
        //    m_scene->Add(std::move(actor));
        //}

        /*{
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
        }*/

        for (int i = 0; i < 50; i++)
        {
        auto actor = CREATE_CLASS_BASE(Actor, "tree");
        actor->name = CreateUnique("tree");
        actor->transform.position = glm::vec3{ randomf(-10, 10), 0, randomf(-10, 10) };
        actor->transform.scale = glm::vec3{ randomf(0.5f, 2.5f), randomf(0.5f, 2.5f), 0 };
        actor->Initialize();
        m_scene->Add(std::move(actor));
        }

        return true;
    }

    void World05::Shutdown()
    {
    }

    void World05::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        //m_transform.rotation.z += 180 * dt;

       auto actor = m_scene->GetActorByName<Actor>("actor1");
       /*
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        actor->transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0;
        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * -dt : 0;
        actor->transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        actor->transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * dt : 0;
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_F) ? 50 * m_speed * dt : 0;
        actor->transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_V) ? 50 * m_speed * -dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_G) ? 50 * m_speed * dt : 0;
        actor->transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_B) ? 50 * m_speed * -dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_H) ? 50 * m_speed * dt : 0;
        actor->transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_N) ? 50 * m_speed * -dt : 0;*/

        m_time += dt;

        auto material = actor->GetComponent<ModelRenderComponent>()->m_material;

        //m_material->ProcessGui();
        //m_material->Bind();
        material->ProcessGui();
        material->Bind();

        material = GET_RESOURCE(Material, "materials/refraction.mtrl");
        if (material)
        {
            ImGui::Begin("Refraction");
            ImGui::DragFloat("IOR", &m_refraction, 0.01f, 1, 3);
            auto program = material->GetProgram();
            program->Use();
            program->SetUniform("ior", m_refraction);
            ImGui::End();
        }
        
        //material->GetProgram()->SetUniform("ambientLight", m_ambientLight);
        

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World05::Draw(Renderer& renderer)
    {


        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_scene->Draw(renderer);
        
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();

    }
}
