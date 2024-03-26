//
// Created by jeol on 20/03/24.
//
#pragma once

#include <queue>

#include <g_app.hpp>
#include <math/vec2.hpp>

namespace ge {
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
    
    enum class RenderShapeType2D {
        RECT,
        CIRCLE,
    }; 

    struct RenderShape2D {
        RenderShapeType2D type;
        Colour colour;
        math::Vec2<float> postion;
        math::Vec2<float> extent;
        float rotation;
    };
    
    enum class RenderObjectType {
        SHAPE_2D,
    };
    
    struct Vertex2D {
        math::Vec2 a_pos;
        math::Vec2 a_uv;
    };
    
    class Renderer {
        public:
            Renderer(g_app::VulkanRenderer backend);
      
            RenderObjID add_shape2d(const RenderShape2D& shape);
            
            void set_clear_colour(const Colour& colour){ m_clear_colour = colour; }

            void draw();
        private:
            void init();

            g_app::VulkanRenderer m_backend;
            g_app::CommandBuffer m_cmd[g_app::VulkanRenderer::MAX_FRAMES_IN_FLIGHT];
            g_app::Buffer<Vertex2D> m_rect_vbuf;
            g_app::Buffer<uint32_t> m_rect_ibuf;
            g_app::Pipeline m_rect_pipeline;

            RenderObjID m_current_id = 1;
            std::queue<RenderObjID> m_freeids; // When a render object is removed its id is added here for use
            std::unordered_map<RenderObjID, RenderShape2D> m_shape2ds; 
            Colour m_clear_colour = Colour(44, 44, 44, 255);
    };
}
