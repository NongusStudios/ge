//
// Created by jandr on 14/03/2024.
//
#include <ge.hpp>

namespace ge {
    Ge::Ge(const GeDesc& desc):
        m_app{g_app::AppInit()
            .set_window_extent(desc.window_extent)
            .configure_vulkan_renderer([&](g_app::VulkanRendererInit& i){
                i.set_enabled_layers({"VK_LAYER_KHRONOS_validation"});
            })
            .init()
        },
        m_renderer{m_app.renderer()},
        m_ent{*this}
    {}


    Ge::~Ge(){
        for(auto& [type, mod] : m_modules){
            delete mod;
        }
    }

    void Ge::run(){
        for(const auto& system : m_startup){
            system(*this);
        }
        m_app.main_loop([&](const std::vector<g_app::Event>& events, const g_app::Time& time){
            for(const auto& event : events){
                m_ent.input(event);
            }
            m_ent.update(time.delta);
            m_renderer.draw();
        });
        m_app.renderer().device_wait_idle();
    }
}
