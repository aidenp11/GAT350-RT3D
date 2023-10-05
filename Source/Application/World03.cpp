#include "World03.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"

#define INTERLEAVE

namespace lady
{
    bool World03::Initialize()
    {

        m_program = GET_RESOURCE(Program, "shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE

        //vertex data
        float vertexData[] = {
            0.75f, 0.75f, 0.0f, 0.0f, 0.75f, 1.0f,
             0.75f, -0.75f, 0.0f, 0.0f, 0.75f, 1.0f,
             -0.75f,  -0.75f, 0.0f, 0.75f, 0.0f, 1.0f,
             -0.75f, 0.75f, 0.0f, 0.75f, 0.0f, 1.0f
        };

        

        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);

        glBindVertexBuffer(0, vbo, 0, sizeof(GLfloat) * 6);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3);
        glVertexAttribBinding(1, 0);
        

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

        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        m_angle += 90 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;
        m_time += dt;

        GLint uniform = glGetUniformLocation(m_program->m_program, "time");
        glUniform1f(uniform, m_time);
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);

        glDrawArrays(GL_QUADS, 0, 4);
 
        // post-render
        renderer.EndFrame();
    }
}
