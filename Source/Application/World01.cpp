#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
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
        m_angleRotate += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_LEFT) ? +(dt * 90) : 0;
        m_angleRotate += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_RIGHT) ? -(dt * 90) : 0;

        m_angleFlip += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_UP) ? +(dt * 90) : 0;
        m_angleFlip += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_DOWN) ? -(dt * 90) : 0;

        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? +dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
        m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;
        m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt : 0;

        m_scale += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? +dt : 0;
        m_scale += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? -dt : 0;

        m_time += dt;
    }

    void World01::Draw(Renderer& renderer)
    {

        // pre-render
        renderer.BeginFrame();

        for (int i = 0; i < 10; i++)
        {

            // render
            glPushMatrix();
            glTranslatef(m_position.x, m_position.y, 0);
            glRotatef(m_angleRotate, 0, 1, 0);
            glRotatef(m_angleFlip, 1, 0, 0);
            glScalef(m_scale, m_scale, 1);

            glBegin(GL_TRIANGLES);

            glColor3f(1, 0, 0);
            glVertex2f(-0.5f, -0.5f);

            glColor3f(0, 1, 0);
            glVertex2f(0, 0.5f);

            glColor3f(0, 0, 1);
            glVertex2f(0.5f, -0.5f);

            glEnd();

            glPopMatrix();
        }

        // post-render
        renderer.EndFrame();
    }
}
