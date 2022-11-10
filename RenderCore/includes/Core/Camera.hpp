#pragma once

#include <glm/vec3.hpp>
#include <glm/ext/matrix_float4x4.hpp>

namespace rec
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

        Camera(const glm::vec3& position = { -1.f, 0.f, 0.f }, const glm::vec3& rotation = { 0.f, 0.f, 0.f }, const ProjectionMode projection_mode = ProjectionMode::Perspective);

        const glm::vec3& get_camera_position() const { return m_position; }
        const glm::vec3& get_camera_rotation() const { return m_rotation; }

        glm::mat4 get_view_matrix();
        glm::mat4 get_projection_matrix();

        void set_position(const glm::vec3& position);
        void move_forward(const float delta);
        void move_right(const float delta);
        void move_up(const float delta);

        void set_rotation(const glm::vec3& rotation);
        void rotate_roll(const float);
        void rotate_pitch(const float);
        void rotate_yaw(const float);

        void set_projection_mode(const ProjectionMode projection_mode);

    private:
        void flag_for_update_view_matrix();
        void update_view_matrix();
        void update_projection_matrix();

        glm::vec3 m_position;
        glm::vec3 m_rotation;
        ProjectionMode m_projection_mode;

        glm::vec3 m_direction{ 0.f, 0.f, 0.f };
        glm::vec3 m_right{ 0.f, 0.f, 0.f };
        glm::vec3 m_up{ 0.f, 0.f, 0.f };

        static constexpr glm::vec3 s_world_up{ 0.f, 0.f, 1.f };
        static constexpr glm::vec3 s_world_right{ 0.f, -1.f, 0.f };
        static constexpr glm::vec3 s_world_forward{ 1.f, 0.f, 0.f };

        glm::mat4 m_view_matrix;
        glm::mat4 m_projection_matrix;

        bool m_need_update_view_matrix = false;
	};
}