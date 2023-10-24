#include "World04.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"

#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace lady
{
    bool World04::Initialize()
    {
        //m_material = GET_RESOURCE(Material, "materials/quad.mtrl");
        auto material = GET_RESOURCE(Material, "materials/multi.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/plane.obj", glm::vec3{ 0 }, glm::vec3{ 0, 0, 0 });

        for (int i = 0; i < 3; i++)
        {
            m_lights[i].type = light_t::eType::Point;
            m_lights[i].position = glm::vec3{ randomf(-5, 5), randomf(1, 8), randomf(-5, 5) };
            m_lights[i].direction = glm::vec3{ 0, -1, 0 };
            m_lights[i].color = glm::vec3{ 1, 1, 1 };
            m_lights[i].intensity = 1;
            m_lights[i].range = 6;
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
        }
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        auto material = m_model->GetMaterial();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0], 0.8f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::End();

        ImGui::Begin("Light");
        const char* types[] = {"Point", "Directional", "Spot"};
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);

        ImGui::ColorEdit3("Ambient Light", &m_ambientLight[0], 0.1f);
        ImGui::ColorEdit3("Diffuse Light", &m_lights[m_selected].color[0], 0.1f);

        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.1f, 0, 10);
        ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f);
        
        if (m_lights[m_selected].type != light_t::Directional) ImGui::DragFloat3("Light Position", &m_lights[m_selected].position[0], 0.1f);
        if (m_lights[m_selected].type != light_t::Point) ImGui::DragFloat3("Light Direction", &m_lights[m_selected].direction[0], 0.1f);
        if (m_lights[m_selected].type == light_t::Spot)
        {
            ImGui::DragFloat("Inner Angle", &m_lights[m_selected].innerAngle, 1, 0, m_lights[m_selected].outerAngle);
            ImGui::DragFloat("Outer Angle", &m_lights[m_selected].outerAngle, 1, m_lights[m_selected].innerAngle, 90);
        }
        ImGui::End();

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? m_speed * -dt : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? m_speed * dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? m_speed * -dt : 0;
        m_transform.position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? m_speed * dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? m_speed * -dt : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? m_speed * dt : 0;
        m_transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_F) ? 50 * m_speed * dt : 0;
        m_transform.rotation.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_V) ? 50 * m_speed * -dt : 0;
        m_transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_G) ? 50 * m_speed * dt : 0;
        m_transform.rotation.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_B) ? 50 * m_speed * -dt : 0;
        m_transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_H) ? 50 * m_speed * dt : 0;
        m_transform.rotation.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_N) ? 50 * m_speed * -dt : 0;

        m_time += dt;

        //m_material->ProcessGui();
        //m_material->Bind();
        material->ProcessGui();
        material->Bind();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "lights[" + std::to_string(i) + "]";

            //m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
            material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            material->GetProgram()->SetUniform(name + ".color", m_lights[i].color);
            material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
            material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            material->GetProgram()->SetUniform(name + ".direction", glm::normalize(m_lights[i].direction));
        }
        
            material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
            material->GetProgram()->SetUniform("ambientLight", m_ambientLight);
        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 4, 5 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        //m_material->GetProgram()->SetUniform("view", view);
        material->GetProgram()->SetUniform("view", view);
        
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        //m_material->GetProgram()->SetUniform("projection", projection);
        material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {


        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        m_model->Draw(GL_TRIANGLES);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();

    }
}
