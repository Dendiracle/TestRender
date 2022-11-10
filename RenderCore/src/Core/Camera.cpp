#include "Core/Camera.hpp"

#include <glm/trigonometric.hpp>
#include <glm/ext/matrix_transform.hpp>

namespace rec
{
    Camera::Camera(const glm::vec3& position, const glm::vec3& rotation, const ProjectionMode projection_mode)
        : m_position(position), m_rotation(rotation), m_projection_mode(projection_mode)
    {
        update_view_matrix();
        update_projection_matrix();
    }

    void Camera::flag_for_update_view_matrix()
    {
        this->m_need_update_view_matrix = true;
    }

    void Camera::update_view_matrix()
    {
        /*float rotate_in_radians_x = glm::radians(-m_rotation.x);
        glm::mat4 rotate_matrix_x(
            1, 0, 0, 0,
            0, cos(rotate_in_radians_x), sin(rotate_in_radians_x), 0,
            0, -sin(rotate_in_radians_x), cos(rotate_in_radians_x), 0,
            0, 0, 0, 1);

        float rotate_in_radians_y = glm::radians(-m_rotation.y);
        glm::mat4 rotate_matrix_y(
            cos(rotate_in_radians_y), 0, -sin(rotate_in_radians_y), 0,
            0, 1, 0, 0,
            sin(rotate_in_radians_y), 0, cos(rotate_in_radians_y), 0,
            0, 0, 0, 1);

        float rotate_in_radians_z = glm::radians(-m_rotation.z);
        glm::mat4 rotate_matrix(
            cos(rotate_in_radians_z), sin(rotate_in_radians_z), 0, 0,
            -sin(rotate_in_radians_z), cos(rotate_in_radians_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1);

        glm::mat4 translate_matrix(
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            -m_position[0], -m_position[1], -m_position[2], 1);

        m_view_matrix = rotate_matrix_y * rotate_matrix_x * translate_matrix;*/

        const float roll_in_radians = glm::radians(m_rotation.x);
        const float pitch_in_radians = glm::radians(m_rotation.y);
        const float yaw_in_radians = glm::radians(m_rotation.z);

        const glm::mat3 rotate_matrix_x(1, 0, 0,
            0, cos(roll_in_radians), sin(roll_in_radians),
            0, -sin(roll_in_radians), cos(roll_in_radians));

        const glm::mat3 rotate_matrix_y(cos(pitch_in_radians), 0, -sin(pitch_in_radians),
            0, 1, 0,
            sin(pitch_in_radians), 0, cos(pitch_in_radians));

        const glm::mat3 rotate_matrix_z(cos(yaw_in_radians), sin(yaw_in_radians), 0,
            -sin(yaw_in_radians), cos(yaw_in_radians), 0,
            0, 0, 1);

        const glm::mat3 euler_rotate_matrix = rotate_matrix_z * rotate_matrix_y * rotate_matrix_x;
        m_direction = glm::normalize(euler_rotate_matrix * s_world_forward);
        m_right = glm::normalize(euler_rotate_matrix * s_world_right);
        m_up = glm::cross(m_right, m_direction);

        m_view_matrix = glm::lookAt(m_position, m_position + m_direction, m_up);
    }

    void Camera::update_projection_matrix()
    {
        if (m_projection_mode == ProjectionMode::Perspective)
        {
            float r = 0.1f;
            float t = 0.1f;
            float f = 100;
            float n = 0.1f;
            m_projection_matrix = glm::mat4
            (
                n / r, 0, 0, 0,
                0, n / t, 0, 0,
                0, 0, (-f - n) / (f - n), -1,
                0, 0, -2 * f * n / (f - n), 0
            );
        }
        else
        {
            float r = 2;
            float t = 2;
            float f = 100;
            float n = 0.1f;
            m_projection_matrix = glm::mat4(1 / r, 0, 0, 0,
                0, 1 / t, 0, 0,
                0, 0, -2 / (f - n), 0,
                0, 0, (-f - n) / (f - n), 1);
        }
    }

    glm::mat4 Camera::get_view_matrix()
    {
        if (m_need_update_view_matrix)
        {
            update_view_matrix();
            m_need_update_view_matrix = false;
        }

        return m_view_matrix;
    }

    glm::mat4 Camera::get_projection_matrix()
    {
        return m_projection_matrix;
    }

    void Camera::set_position(const glm::vec3& position)
    {
        m_position = position;
        this->flag_for_update_view_matrix();
    }

    void Camera::move_forward(const float delta)
    {
        m_position += m_direction * delta;
        this->flag_for_update_view_matrix();
    }

    void Camera::move_right(const float delta)
    {
        //logInfo("00 {0} {1} {2} ", m_position.x, m_position.y, m_position.z);
        //logInfo("01 {0} {1} {2} ", m_right.x, m_right.y, m_right.z);
        m_position += m_right* delta;
        this->flag_for_update_view_matrix();
        //logInfo("10 {0} {1} {2} ", m_position.x, m_position.y, m_position.z);
        //logInfo("11 {0} {1} {2} ", m_right.x, m_right.y, m_right.z);
    }

    void Camera::move_up(const float delta)
    {
        m_position += m_up * delta;
        this->flag_for_update_view_matrix();
    }

    void Camera::set_rotation(const glm::vec3& rotation)
    {
        m_rotation = rotation;
        this->flag_for_update_view_matrix();
    }

    void Camera::rotate_roll(const float angle)
    {
        m_rotation.z += angle;
        this->flag_for_update_view_matrix();
    }

    void Camera::rotate_pitch(const float angle)
    {
        m_rotation.y -= angle;
        this->flag_for_update_view_matrix();
    }

    void Camera::rotate_yaw(const float angle)
    {
        m_rotation.z += angle;
        this->flag_for_update_view_matrix();
    }

    void Camera::set_projection_mode(const ProjectionMode projection_mode)
    {
        m_projection_mode = projection_mode;
        update_projection_matrix();
    }
}