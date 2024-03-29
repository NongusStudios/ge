#include <renderer.hpp>

namespace ge {
    Renderer::Renderer(g_app::VulkanRenderer backend): m_backend{backend} {
        init();
        
    }

    void Renderer::draw(){
        if(!m_backend.acquire_next_swapchain_image()) return; 

        m_cmd[m_backend.current_frame()]
            .begin()
            .begin_default_render_pass(m_clear_colour.r, 
                                       m_clear_colour.g,
                                       m_clear_colour.b, 
                                       m_clear_colour.a)
            .cmd([&](g_app::CommandBuffer& cmd){
                for(auto& [type, render_data] : m_render_data){
                std::vector<RenderObject*> objs = {};
                objs.reserve(render_data.second.size());
                    for(auto& [id, robj] : render_data.second){
                        objs.push_back(robj);
                    }
                    render_data.first->draw(cmd, objs);
                }
            })
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
        m_desc_pool = g_app::DescriptorPoolInit()
            .set_max_sets(1000)
            .add_pool_size(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 250)
            .add_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 250)
            .add_pool_size(VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 250)
            .add_pool_size(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 250)
            .init(m_backend);

        for(uint32_t i = 0; i < g_app::VulkanRenderer::MAX_FRAMES_IN_FLIGHT; i++){
            m_cmd[i] = g_app::CommandBuffer(m_backend);
        }
    }
    
    RectData::RectData(Renderer& renderer): RenderData(renderer) {
        auto backend = m_renderer.backend();

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
            .init(backend);

        m_rect_ibuf = g_app::BufferInit<uint32_t>()
            .set_memory_usage(VMA_MEMORY_USAGE_CPU_TO_GPU)
            .set_usage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT)
            .set_size(6)
            .set_data(rect_indices)
            .set_label("m_rect_ibuf")
            .init(backend);
        
        m_transform_sbuf = g_app::BufferInit<math::Mat4>()
            .set_memory_usage(VMA_MEMORY_USAGE_CPU_TO_GPU)
            .set_usage(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT)
            .set_size(1000)
            .set_label("m_transform_sbuf")
            .init(backend);

        m_rect_pipeline = g_app::GraphicsPipelineInit()
            .attach_shader_module(g_app::ShaderModuleInit()
                    .set_stage(VK_SHADER_STAGE_VERTEX_BIT)
                    .set_src_from_file("../src/shaders/rect.vert.spv")
                    .set_label("m_rect_vertex_shader")
                    .init(backend))
            .attach_shader_module(g_app::ShaderModuleInit()
                    .set_stage(VK_SHADER_STAGE_FRAGMENT_BIT)
                    .set_src_from_file("../src/shaders/rect.frag.spv")
                    .set_label("m_rect_fragment_shader")
                    .init(backend))
            .add_vertex_binding(g_app::VertexBindingBuilder(sizeof(Vertex2D))
                .add_vertex_attribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, a_pos))
                .add_vertex_attribute(VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, a_uv))
                .build()
            )
            .set_render_pass(backend.default_render_pass())
            .init(backend);

    }
    
    void RectData::draw(g_app::CommandBuffer& cmd, std::vector<RenderObject*> objects){
        
    }
}
