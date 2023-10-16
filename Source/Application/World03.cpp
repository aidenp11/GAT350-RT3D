#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include "Renderer/VertexBuffer.h"

#include <glm/glm/gtc/type_ptr.hpp>

#define INTERLEAVE

namespace lady
{
    bool World03::Initialize()
    {
        m_material = GET_RESOURCE(Material, "materials/quad.mtrl");

        //m_program = GET_RESOURCE(Program, "shaders/unlit_texture.prog");
        //m_program->Use();

        //m_texture = GET_RESOURCE(Texture, "Textures/llama.jpg");
        //m_texture->Bind();
        //m_texture->SetActive(GL_TEXTURE0);

#ifdef INTERLEAVE

        //vertex data
        float vertexData[] = {
            0.75f, 0.75f, 0.0f, 0.0f, 0.75f, 1.0f, 0.0f, 0.0f,
             0.75f, -0.75f, 0.0f, 0.0f, 0.75f, 1.0f, 0.0f, 1.0f,
             -0.75f,  -0.75f, 0.0f, 0.75f, 0.0f, 1.0f, 1.0f, 1.0f,
             -0.75f, 0.75f, 0.0f, 0.75f, 0.0f, 1.0f, 1.0f, 0.0f
        };

        

        //gluint vbo;
        //glgenbuffers(1, &vbo);

        //glbindbuffer(gl_array_buffer, vbo);
        //glbufferdata(gl_array_buffer, sizeof(vertexdata), vertexdata, gl_static_draw);

        //glgenvertexarrays(1, &m_vao);
        //glbindvertexarray(m_vao);

        //glenablevertexattribarray(0);
        //glvertexattribformat(0, 3, gl_float, gl_false, 0);

        //glbindvertexbuffer(0, vbo, 0, sizeof(glfloat) * 8);

        //glenablevertexattribarray(0);
        //glvertexattribformat(0, 3, gl_float, gl_false, 0);
        //glvertexattribbinding(0, 0);

        //glenablevertexattribarray(1);
        //glvertexattribformat(1, 3, gl_float, gl_false, sizeof(glfloat) * 3);
        //glvertexattribbinding(1, 0);

        //glenablevertexattribarray(2);
        //glvertexattribformat(2, 2, gl_float, gl_false, sizeof(glfloat) * 6);
        //glvertexattribbinding(2, 0);

        m_vertexBuffer = GET_RESOURCE(VertexBuffer, "vb");
        m_vertexBuffer->CreateVertexBuffer(sizeof(vertexData), 4, vertexData);
        m_vertexBuffer->SetAttribute(0, 3, 8 * sizeof(GLfloat), 0);                  // position 
        m_vertexBuffer->SetAttribute(1, 3, 8 * sizeof(GLfloat), 3 * sizeof(float));  // color 
        m_vertexBuffer->SetAttribute(2, 2, 8 * sizeof(GLfloat), 6 * sizeof(float));  // texcoord
        

#else

        //vertex data
        float positionData[] = {
            0.25f, 0.25f, 0.0f,
             0.25f, -0.25f, 0.0f,
             -0.25f,  -0.25f, 0.0f,
             -0.25f, 0.25f, 0.0f
        };

        float colorData[] =
        {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        GLuint vbo[2];
        glGenBuffers(2, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, sizeof(GLfloat) * 3);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, sizeof(GLfloat) * 3);

#endif
        //m_position.z = -10.0f;

        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();

        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
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

        m_material->ProcessGui();
        m_material->Bind();
        //m_program->SetUniform("offset", glm::vec2{ 0, 0 });
        //m_program->SetUniform("tiling", glm::vec2{ 2, 2 });

        // model matrix
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, glm::radians(m_angle), glm::vec3{ 0, 0, 1 });
        //glm::mat4 model = position * rotation;
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
        
        //view matrix
        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 4, 5 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 });
        m_material->GetProgram()->SetUniform("view", view);
        
        // projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        m_material->GetProgram()->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World03::Draw(Renderer& renderer)
    {
        //// pre-render
        //renderer.BeginFrame();

        //// render
        //glBindVertexArray(m_vao);

        //glDrawArrays(GL_QUADS, 0, 4);

        //ENGINE.GetSystem<Gui>()->Draw();
 
        //// post-render
        //renderer.EndFrame();

        // pre-render
        renderer.BeginFrame();
        // render
        m_vertexBuffer->Draw(GL_TRIANGLE_STRIP);
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();

    }
}
