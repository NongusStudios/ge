//
// Created by jandr on 14/03/2024.
//
#pragma once

#include <cstdint>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <cassert>

namespace ge { class Ge; }

namespace ge::ent {
    using Entity = uint64_t;
    using ComponentId = uint64_t;
    using ComponentTypeId = uint32_t;

    class Ent;
    class Component {
    public:
        Component() = default;

        virtual void update(Ge& ge, Entity entity, double delta){}

        ComponentId id() const {
            return m_id;
        }
    private:
        ComponentId m_id = 0;

        friend class Ent;
    };

    class Ent {
    public:
        ~Ent(){
            for(auto& [type, component_array] : m_components) {
                for (auto &[entity, component]: component_array) {
                    delete component;
                }
            }
        }

        template<typename T>
        void register_component(){
            assert(!m_components.contains(typeid(T)));
            m_components[typeid(T)] = {};
        }

        Entity create_entity(){
            return m_current_entity_id++;
        }

        template<typename T>
        void spawn_component(Entity e, const Component& c){
            assert(m_components.contains(typeid(T)));

            m_components[typeid(T)][e] = new T();

            ComponentId id = m_current_component_id++;
            m_components[typeid(T)][e]->m_id = id;
            m_entities[id] = e;
        }

        template<typename T>
        Component& retrieve_component(Entity e){
            assert(m_components.contains(typeid(T)));
            return *m_components[typeid(T)][e];
        }

        template<typename T>
        Entity retrieve_entity(const Component& c){
            assert(m_entities.contains(c.id()));
            return m_entities[c.id()];
        }

        void update(Ge& ge, double delta);
    private:
        Entity m_current_entity_id = 1;
        ComponentId m_current_component_id = 1;
        std::unordered_map<std::type_index, // Component type
            std::unordered_map<Entity, Component*> // Components mapped to entities
        > m_components;
        std::unordered_map<ComponentId, Entity> m_entities; // Entities mapped to ComponentIds
    };
}
