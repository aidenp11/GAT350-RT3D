#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace lady
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_angle += 90 * dt;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;
        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glPushMatrix();
        glTranslatef(m_position.x, m_position.y, 0);
        glRotatef(m_angle, 1, 1, 1);
        glScalef(sin(m_time) / 2, sin(m_time) / 2, sin(m_time) / 2);

        glBegin(GL_TRIANGLES);

            glColor3f(1, 0.94, 0);
            glVertex2f(-0.5f, -0.5f);

            glColor3f(0.3, 1, 0.1);
            glVertex2f(0, 0.5f);

            glColor3f(1, 0.1, 1);
            glVertex2f(0.5f, -0.5f);

        glEnd();

        glPopMatrix();

        glPushMatrix();

        glBegin(GL_LINES);

        for (int i = 0; i < 100; i++)
        {
            glColor3f(randomf(), randomf(), randomf());
            glVertex2f(randomf(-1.0f, 1.0f), randomf(-1.0f, 1.0f));

            glColor3f(randomf(), randomf(), randomf());
            glVertex2f(randomf(-1.0f, 1.0f), randomf(-1.0f, 1.0f));
        }

        glEnd();

        glPopMatrix();
        // post-render
        renderer.EndFrame();
    }
}
