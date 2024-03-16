//
// Created by jandr on 14/03/2024.
//
#pragma once

#include <ent.hpp>

#include <g_app.hpp>

namespace ge {
    class Ge;
    struct GeDesc {
        g_app::Extent2D<uint32_t> window_extent = {800, 600};
        std::vector<std::function<void(Ge&)>> startup_systems = {};
    };

    class Ge {
    public:
        explicit Ge(const GeDesc& desc);

        void run();

        ent::Ent& ent() { return m_ent; }
    private:
        g_app::App m_app;
        ent::Ent m_ent;
        std::vector<std::function<void(Ge&)>> m_startup;
    };

    class GeInit {
    public:
        GeInit& set_window_extent(uint32_t width, uint32_t height){
            m_desc.window_extent = {width, height};
            return *this;
        }

        GeInit& add_startup_system(const std::function<void(Ge&)>& system){
            m_desc.startup_systems.push_back(system);
            return *this;
        }

        Ge init(){
            return Ge{m_desc};
        }
    private:
        GeDesc m_desc = {};
    };
}
