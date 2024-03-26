//
// Created by jandr on 14/03/2024.
//
#pragma once

#include <ent.hpp>
#include <renderer.hpp>
#include <g_app.hpp>

#include <vector>

namespace ge {
    class Ge;
    struct GeDesc {
        g_app::Extent2D<uint32_t> window_extent = {800, 600};
    };

    class Module {
    public:
        virtual void init(Ge& ge){}
        virtual void update(Ge& ge, double delta){}
        virtual void input(Ge& ge, const g_app::Event& event){}
    };

    class Ge {
    public:
        explicit Ge(const GeDesc& desc);

        ~Ge();

        void run();
        void quit(){
            m_app.window().quit();
        } 

        template<typename T>
        void add_module(const T& mod){
            assert(!m_modules.contains(typeid(T)));
            m_modules[typeid(T)] = new T(mod);
        }

        void add_startup_system(std::function<void(Ge&)> system){
            m_startup.push_back(system);
        }

        ent::Ent& ent() { return m_ent; }
        Renderer& renderer() { return m_renderer; }
    private: 
        g_app::App m_app;
        ent::Ent m_ent;
        Renderer m_renderer;
        std::vector<std::function<void(Ge&)>> m_startup;
        std::unordered_map<std::type_index, Module*> m_modules;
    }; 
}
