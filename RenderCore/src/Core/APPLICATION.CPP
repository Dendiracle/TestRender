#include "Core/Application.hpp"

#include "Core/Utils/Log.hpp"
#include "Core/Resources/ResourceManager.hpp"

#include "Core/Camera.hpp"
#include "Core/ApplicationWindow/WindowOpenGL.hpp"
#include "Core/Rendering/OpenGL/Renderer.hpp"
#include "Core/Rendering/OpenGL/ShaderProgram.hpp"
#include "Core/Rendering/OpenGL/VertexBuffer.hpp"
#include "Core/Rendering/OpenGL/VertexArray.hpp"
#include "Core/Rendering/OpenGL/IndexBuffer.hpp"
#include "Core/Rendering/OpenGL/Texture2D.hpp"

#include <glm/mat3x3.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GLFW/glfw3.h>

namespace rec
{
    GLfloat positions_colors2[] = 
    {
       -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 1.0f,
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f
    };

    GLfloat positions_colors_coords[] = 
    {
         0.0f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,   0.f, 0.f,
         0.0f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,  1.f, 0.f,
         0.0f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,   0.f,  1.f,
         0.0f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,  1.f,  1.f
    };

    GLuint indices[] = {
        0, 1, 2, 3, 2, 1
    };

    /*const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "layout(location = 2) in vec2 texture_coord;"
        "uniform mat4 model_matrix;"
        "uniform mat4 view_projection_matrix;"
        "uniform int current_frame;"
        "out vec3 color;"
        "out vec2 tex_coord_smile;"
        "out vec2 tex_coord_quads;"
        "void main() {"
        "   color = vertex_color;"
        "   tex_coord_smile = texture_coord;"
        "   tex_coord_quads = texture_coord + vec2(current_frame / 1000.f, current_frame / 1000.f);"
        "   gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0F);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "in vec2 tex_coord_smile;"
        "in vec2 tex_coord_quads;"
        "layout(binding = 0) uniform sampler2D InTexture_Smile;"
        "layout(binding = 1) uniform sampler2D InTexture_Quads;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = texture(InTexture_Smile, tex_coord_smile) * texture(InTexture_Quads, tex_coord_quads);"
        "}";*/
    const char* vertex_shader =
        "#version 460\n"
        "layout(location = 0) in vec3 vertex_position;"
        "layout(location = 1) in vec3 vertex_color;"
        "layout(location = 2) in vec2 texture_coord;"
        "uniform mat4 model_matrix;"
        "uniform mat4 view_projection_matrix;"
        "uniform int current_frame;"
        "out vec3 color;"
        "out vec2 coord;"
        "void main() {"
        "   color = vertex_color;"
        "   coord = texture_coord;"
        "   gl_Position = view_projection_matrix * model_matrix * vec4(vertex_position, 1.0F);"
        "}";

    const char* fragment_shader =
        "#version 460\n"
        "in vec3 color;"
        "in vec2 coord;"
        "uniform sampler2D ourTexture;"
        "out vec4 frag_color;"
        "void main() {"
        "   frag_color = texture(ourTexture, coord);"
        "}";

    std::unique_ptr<ShaderProgram> p_shader_program;
    std::unique_ptr<VertexBuffer> p_positions_colors_vbo;
    std::unique_ptr<IndexBuffer> p_index_buffer;
    std::unique_ptr<Texture2D> p_texture_smile;
    std::unique_ptr<Texture2D> p_texture_quads;
    std::unique_ptr<VertexArray> p_vao;

    float scale[3] = { 1.f, 1.f, 1.f };
    float rotate = 0.f;
    float translate[3] = { 0.f, 0.f, 0.f };
    bool perspective_camera = true;


    unsigned int prevX;
    unsigned int prevY;

	Application::Application(char** argv)
	{
        ResourceManager::setExecutablePath(argv[0]);
	}

	Application::~Application()
	{
	}

    Camera Application::get_camera()
    {
        return *this->m_pCamera;
    }

    int Application::start_window(unsigned int windowWidth, unsigned int windowHeight, const char* title)
	{

        this->m_pCamera = new Camera();
		this->m_pWindow = std::make_unique<rec::WindowOpenGL>(windowWidth, windowHeight, title);
        ResourceManager::loadTexture("Keks", "res/Textures/Gr_Fs.png");

        m_event_dispatcher.add_event_listener<EventMouseMoved>(
            [](EventMouseMoved& event)
            {
                const int aa[2]{ Input::GetPreviousMouseXPosition(), Input::GetPreviousMouseYPosition() };
                const int a[2]{ event.x, event.y };
                Input::SetCurrentMousePositions(a);
            });

        m_event_dispatcher.add_event_listener<EventWindowResize>(
            [](EventWindowResize& event)
            {
                logInfo("[Resized] Changed size to {0}x{1}", event.width, event.height);
            });

        m_event_dispatcher.add_event_listener<EventWindowClose>(
            [&](EventWindowClose& event)
            {
                logInfo("[WindowClose]");
                m_should_window_close = true;
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonPressed>(
            [&](EventMouseButtonPressed& event)
            {
                //logInfo("[Mouse button pressed: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::PressMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, true);
            });

        m_event_dispatcher.add_event_listener<EventMouseButtonReleased>(
            [&](EventMouseButtonReleased& event)
            {
                //logInfo("[Mouse button released: {0}, at ({1}, {2})", static_cast<int>(event.mouse_button), event.x_pos, event.y_pos);
                Input::ReleaseMouseButton(event.mouse_button);
                on_mouse_button_event(event.mouse_button, event.x_pos, event.y_pos, false);
            });

        m_event_dispatcher.add_event_listener<EventKeyPressed>(
            [&](EventKeyPressed& event)
            {
                Input::PressKey(event.key_code);
            });

        m_event_dispatcher.add_event_listener<EventKeyReleased>(
            [&](EventKeyReleased& event)
            {
                Input::ReleaseKey(event.key_code);
            });

        m_pWindow->set_event_callback(
            [&](BaseEvent& event)
            {
                m_event_dispatcher.dispatch(event);
            }
        );

        p_shader_program = std::make_unique<ShaderProgram>(vertex_shader, fragment_shader);
        if (!p_shader_program->is_compiled())
        {
            return false;
        }

        BufferLayout buffer_layout_2vec3
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3
        };

        BufferLayout buffer_layout_vec3_vec3_vec2
        {
            ShaderDataType::Float3,
            ShaderDataType::Float3,
            ShaderDataType::Float2
        };

        std::shared_ptr<Texture2D> textureKeks = ResourceManager::getTexture("Keks");
        p_vao = std::make_unique<VertexArray>();
        p_positions_colors_vbo = std::make_unique<VertexBuffer>(positions_colors_coords, sizeof(positions_colors_coords), buffer_layout_vec3_vec3_vec2);
        p_index_buffer = std::make_unique<IndexBuffer>(indices, sizeof(indices) / sizeof(GLuint));
        p_vao->add_vertex_buffer(*p_positions_colors_vbo);

        p_vao->set_index_buffer(*p_index_buffer);     
        
        static int count = 0;

		while (!m_should_window_close)
		{
            Renderer_OpenGL::set_clear_color(.2F, .2F, .2F, .0F);
            Renderer_OpenGL::clear();

            p_shader_program->bind();

            glm::mat4 scale_matrix(
                scale[0], 0, 0, 0,
                0, scale[1], 0, 0,
                0, 0, scale[2], 0,
                0, 0, 0, 1);
            float rotate_in_radians = glm::radians(rotate);
            glm::mat4 rotate_matrix(cos(rotate_in_radians), sin(rotate_in_radians), 0, 0,
                -sin(rotate_in_radians), cos(rotate_in_radians), 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1);
            glm::mat4 translate_matrix(1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                translate[0], translate[1], translate[2], 1);

            glm::mat4 model_matrix = translate_matrix * rotate_matrix * scale_matrix;
            p_shader_program->set_matrix4("model_matrix", model_matrix);
            p_shader_program->set_int("current_frame", count++);

            m_pCamera->set_projection_mode(perspective_camera ? Camera::ProjectionMode::Perspective : Camera::ProjectionMode::Orthographic);
            p_shader_program->set_matrix4("view_projection_matrix", m_pCamera->get_projection_matrix() * m_pCamera->get_view_matrix());
            textureKeks->bind();
            Renderer_OpenGL::draw(*p_vao);
            
			m_pWindow->onUpdate();

            if (Input::IsKeyPressed(KeyCode::KEY_Z))
            {
                m_pCamera->set_position(glm::vec3(0, 0, 0));
                m_pCamera->set_rotation(glm::vec3(0, 0, 0));
            }
            else if (Input::IsMouseButtonPressed(rec::MouseButton::MOUSE_BUTTON_1))
            {
                if (Input::IsKeyPressed(KeyCode::KEY_W))
                {
                    m_pCamera->move_forward(.02f);
                }
                if (Input::IsKeyPressed(KeyCode::KEY_S))
                {
                    m_pCamera->move_forward(-.02f);
                }
                if (Input::IsKeyPressed(KeyCode::KEY_D))
                {
                    m_pCamera->move_right(.02f);
                }
                if (Input::IsKeyPressed(KeyCode::KEY_A))
                {
                    m_pCamera->move_right(-.02f);
                }                

                const int PrevPos[2] = { Input::GetPreviousMouseXPosition(), Input::GetPreviousMouseYPosition() };
                const int CurrPos[2] = { Input::GetCurrentMouseXPosition(), Input::GetCurrentMouseYPosition() };

                if (PrevPos[0] != CurrPos[0])
                {
                    m_pCamera->rotate_yaw((PrevPos[0] - CurrPos[0]) / 4.f);
                }
                if (PrevPos[1] != CurrPos[1])
                {
                    m_pCamera->rotate_pitch((PrevPos[1] - CurrPos[1]) / 4.f);
                }
            }

            const int PrevPos1[2] = { Input::GetPreviousMouseXPosition(), Input::GetPreviousMouseYPosition() };
            const int CurrPos1[2] = { Input::GetCurrentMouseXPosition(), Input::GetCurrentMouseYPosition() };

            if (PrevPos1[0] != CurrPos1[0] || PrevPos1[1] != CurrPos1[1])
            {
                Input::SetPreviousMousePositions(CurrPos1);
            }
		}

        this->m_pWindow = nullptr;

		return 0;
	}
}