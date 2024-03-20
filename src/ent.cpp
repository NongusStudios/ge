//
// Created by jandr on 14/03/2024.
//
#include <ent.hpp>

#include <ge.hpp>

namespace ge::ent {
    void Ent::update(double delta){
        for(auto& [type, component_array] : m_components){
            for(auto& [entity, component] : component_array){
                component->update(m_ge, entity, delta);
            }
        }
    }

    void Ent::input(const g_app::Event& event){
        for(auto& [type, component_array] : m_components){
            for(auto& [entity, component] : component_array){
                component->input(m_ge, entity, event);
            }
        }
    }
}
