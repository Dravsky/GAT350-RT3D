#include "World07.h"
#include "Framework/Framework.h"
#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/gtx/color_space.hpp>

namespace nc
{
    bool World07::Initialize()
    {
        m_scene = std::make_unique<Scene>();
        m_scene->Load("scenes/scene_FrameBuffer.json");
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

        return true;
    }

    void World07::Shutdown()
    {

    }

    void World07::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();

        m_scene->Update(dt);
        m_scene->ProcessGui();

        // set postprocess gui
        ImGui::Begin("Post-Process");
        ImGui::SliderFloat("Blend", &m_blend, 0, 1);

        bool effect = m_params & INVERT_MASK;
        if (ImGui::Checkbox("Invert", &effect)) 
        {
            (effect) ? m_params |= INVERT_MASK : m_params &= ~INVERT_MASK;
        }

        effect = m_params & GRAYSCALE_MASK;
        if (ImGui::Checkbox("Grayscale", &effect))
        {
            (effect) ? m_params |= GRAYSCALE_MASK : m_params &= ~GRAYSCALE_MASK;
        }

        effect = m_params & COLORTINT_MASK;
        if (ImGui::Checkbox("Colortint", &effect))
        {
            (effect) ? m_params |= COLORTINT_MASK : m_params &= ~COLORTINT_MASK;
        }
        ImGui::ColorEdit3("Color", glm::value_ptr(m_tint));

        effect = m_params & GRAIN_MASK;
        if (ImGui::Checkbox("Grain", &effect))
        {
            (effect) ? m_params |= GRAIN_MASK : m_params &= ~GRAIN_MASK;
        }
        ImGui::DragFloat("Graininess", &m_graininess, 0.1f, 0, 5);

        effect = m_params & SCANLINE_MASK;
        if (ImGui::Checkbox("Scanline", &effect))
        {
            (effect) ? m_params |= SCANLINE_MASK : m_params &= ~SCANLINE_MASK;
        }
        ImGui::DragFloat("Scanline Width", &m_scalineWidth, 0.1f, 2, 10);
        ImGui::DragFloat("Scanline Intensity", &m_scanlineIntensity, 0.1f, 0, 5);

        effect = m_params & RAVE_MASK;
        if (ImGui::Checkbox("Rave", &effect))
        {
            (effect) ? m_params |= RAVE_MASK : m_params &= ~RAVE_MASK;
        }

        ImGui::End();

        // set postprocess shader
        auto program = GET_RESOURCE(Program, "shaders/postprocess.prog");
        if (program)
        {
            program->Use();
            program->SetUniform("blend", m_blend);
            program->SetUniform("params", m_params);
            program->SetUniform("tint", m_tint);
            program->SetUniform("graininess", m_graininess);
            program->SetUniform("scanlinewidth", m_scalineWidth);
            program->SetUniform("scanlineintensity", m_scanlineIntensity);
            program->SetUniform("time", m_time);
        }

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World07::Draw(Renderer& renderer)
    {
        // *** PASS 1 ***
        m_scene->GetActorByName("postprocess")->active = false;

        auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        renderer.SetViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.BeginFrame({ 0, 0, 0 });
        m_scene->Draw(renderer);
        
        framebuffer->Unbind();

        // *** PASS 2 ***
        m_scene->GetActorByName("postprocess")->active = true;

        renderer.ResetViewport();
        renderer.BeginFrame();
        m_scene->GetActorByName("postprocess")->Draw(renderer);

        // post-render
        ENGINE.GetSystem<Gui>()->Draw();
        renderer.EndFrame();
    }
}