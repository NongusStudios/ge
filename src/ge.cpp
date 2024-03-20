//
// Created by jandr on 14/03/2024.
//
#include <ge.hpp>

namespace ge {
    Ge::Ge(const GeDesc& desc):
        m_app{g_app::AppInit()
            .set_window_extent(desc.window_extent)
            .init()
        },
        m_startup{desc.startup_systems},
        m_ent{*this}
    {}

    void Ge::run(){
        for(const auto& system : m_startup){
            system(*this);
        }
        m_app.main_loop([&](const std::vector<g_app::Event>& events, const g_app::Time& time){
            for(const auto& event : events){
                m_ent.input(event);
            }
            m_ent.update(time.delta);
        });
    }
}