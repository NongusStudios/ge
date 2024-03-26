//
// Created by jeol on 20/03/24.
//
#pragma once

#include <queue>
#include <typeindex>

#include <g_app.hpp>
#include <math/vec2.hpp>
#include <math/mat4.hpp>

namespace ge {
    class Renderer;

    struct Colour { float r, g, b, a; 
        Colour(): Colour(0, 0, 0, 255) {}
        Colour(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a):
            r{float(_r)/255.0f},
            g{float(_g)/255.0f},
            b{float(_b)/255.0f},
            a{float(_a)/255.0f}
        {}
    };
    
    using RenderObjID = uint64_t; 
    
    struct Vertex2D {
        math::Vec2 a_pos;
        math::Vec2 a_uv;
    };
    
    class RenderObject {
    public:
    };
    
    class RenderData {
    public:
        RenderData(Renderer& renderer): m_renderer{renderer} {}
        Renderer& m_renderer;
    
        virtual void draw(g_app::CommandBuffer& cmd, std::vector<RenderObject*> objects){}
    };

    // Shape2D
    
    class RectData : public RenderData {
    public:
        struct PushConstant {
            math::Mat4 view;
        };

        RectData(Renderer& backend);
        g_app::Buffer<Vertex2D> m_rect_vbuf;
        g_app::Buffer<uint32_t> m_rect_ibuf;
        g_app::Buffer<math::Mat4> m_transform_sbuf;
        g_app::Buffer<PushConstant> m_rect_ubuf;
        g_app::DescriptorSet m_desc_set;
        g_app::Pipeline m_rect_pipeline;

        void draw(g_app::CommandBuffer& cmd, std::vector<RenderObject*> objects) override;
    };

    class Rect : public RenderObject {
    public:
    };
    
    class Renderer {
        public:
            Renderer(g_app::VulkanRenderer backend);
            ~Renderer(){
                for(auto& [ti, pair] : m_render_data){
                    delete pair.first;
                    for(auto& [rid, ro] : pair.second){
                        delete ro;
                    }
                }
            }

            void set_clear_colour(const Colour& colour){ m_clear_colour = colour; }
            
            template<typename T>
            void register_render_data(){
                assert(!m_render_data.contains(typeid(T)));
                m_render_data[typeid(T)] = {new T(*this), {}};
            }

            template<typename T>
            RenderObjID add_render_object(const T& obj){
                assert(m_render_data.contains(typeid(T)));
                auto& [render_data, render_objs] = m_render_data[typeid(T)];
                
                RenderObjID id = 0;
                if(m_freeids.empty()) id = m_current_id++;
                else { id = m_freeids.front(); m_freeids.pop(); }

                render_objs[id] = new T(obj);
                return id;
            }

            template<typename T>
            void remove_render_object(RenderObjID id){
                assert(m_render_data.contains(typeid(T)));
                assert(m_render_data[typeid(T)].second.contains(id));
                m_render_data[typeid(T)].second.erase(id);
            }

            template<typename T>
            T& get_render_object(RenderObjID id){
                assert(m_render_data.contains(typeid(T)));
                assert(m_render_data[typeid(T)].second.contains(id));
                return *reinterpret_cast<T*>(m_render_data[typeid(T)].second[id]);
            }

            void draw();

            g_app::VulkanRenderer backend() { return m_backend; }
            g_app::DescriptorPool desc_pool() { return m_desc_pool; }
        private:
            void init();

            g_app::VulkanRenderer m_backend;
            g_app::DescriptorPool m_desc_pool;
            g_app::CommandBuffer m_cmd[g_app::VulkanRenderer::MAX_FRAMES_IN_FLIGHT]; 

            RenderObjID m_current_id = 1;
            std::queue<RenderObjID> m_freeids; // When a render object is removed its id is added here for use
            std::unordered_map<std::type_index, std::pair<RenderData*,
                std::unordered_map<RenderObjID, RenderObject*>>> m_render_data;
            Colour m_clear_colour = Colour(44, 44, 44, 255);
    };
}
