#include <renderer.hpp>

namespace ge {
    Renderer::Renderer(g_app::VulkanRenderer backend): m_backend{backend} {
        init();
        
    }

    RenderObjID Renderer::add_shape2d(const RenderShape2D& shape){
        RenderObjID id = 0; 
        if(!m_freeids.empty()){ id = m_freeids.front(); m_freeids.pop(); }
        else { id = m_current_id++; }

        m_shape2ds[id] = shape;
        return id;
    }

    void Renderer::draw(){
        if(!m_backend.acquire_next_swapchain_image()) return; 

        m_cmd[m_backend.current_frame()]
            .begin()
            .begin_default_render_pass(m_clear_colour.r, 
                                       m_clear_colour.g,
                                       m_clear_colour.b, 
                                       m_clear_colour.a)
            .bind_pipeline(m_rect_pipeline, VK_PIPELINE_BIND_POINT_GRAPHICS)
            .bind_vertex_buffer(m_rect_vbuf)
            .bind_index_buffer(m_rect_ibuf, VK_INDEX_TYPE_UINT32)
            .draw_indexed(6, 1)
            .end_render_pass()
            .submit(g_app::Queue::GRAPHICS, {
                {m_backend.current_image_available_semaphore()},
                {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
                {m_backend.current_render_finished_semaphore()},
                m_backend.current_in_flight_fence()
            });
        m_backend.present();
    }
    void Renderer::init(){
        Vertex2D rect_vertices[] = {
            {{-1.0f, -1.0f}, {0.0f, 0.0f}}, // Top Left
            {{ 1.0f, -1.0f}, {1.0f, 0.0f}}, // Top Right
            {{ 1.0f,  1.0f}, {1.0f, 1.0f}}, // Bottom Right
            {{-1.0f,  1.0f}, {0.0f, 1.0f}}, // Bottom Left
        };
        uint32_t rect_indices[] = {
            3, 0, 1,
            1, 2, 3,
        };

        m_rect_vbuf = g_app::BufferInit<Vertex2D>()
            .set_memory_usage(VMA_MEMORY_USAGE_CPU_TO_GPU)
            .set_usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
            .set_size(4)
            .set_data(rect_vertices)
            .set_label("m_rect_vbuf")
            .init(m_backend);

        m_rect_ibuf = g_app::BufferInit<uint32_t>()
            .set_memory_usage(VMA_MEMORY_USAGE_CPU_TO_GPU)
            .set_usage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
            .set_size(6)
            .set_data(rect_indices)
            .set_label("m_rect_ibuf")
            .init(m_backend);

        m_rect_pipeline = g_app::GraphicsPipelineInit()
            .attach_shader_module(g_app::ShaderModuleInit()
                    .set_stage(VK_SHADER_STAGE_VERTEX_BIT)
                    .set_src_from_file("../src/shaders/rect.vert.spv")
                    .set_label("m_rect_vertex_shader")
                    .init(m_backend))
            .attach_shader_module(g_app::ShaderModuleInit()
                    .set_stage(VK_SHADER_STAGE_FRAGMENT_BIT)
                    .set_src_from_file("../src/shaders/rect.frag.spv")
                    .set_label("m_rect_fragment_shader")
                    .init(m_backend))
            .add_vertex_binding(g_app::VertexBindingBuilder(sizeof(Vertex2D))
                .add_vertex_attribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, a_pos))
                .add_vertex_attribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, a_uv))
                .build()
            )
            .set_render_pass(m_backend.default_render_pass())
            .init(m_backend);
            
            

        for(uint32_t i = 0; i < g_app::VulkanRenderer::MAX_FRAMES_IN_FLIGHT; i++){
            m_cmd[i] = g_app::CommandBuffer(m_backend);
        }
    }

}
