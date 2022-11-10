#pragma once

#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <string>
#include <map>

namespace rec 
{
    class Texture2D 
    {
    public:

        struct SubTexture2D
        {
            glm::vec2 leftBottomUV;
            glm::vec2 rightTopUV;

            SubTexture2D(const glm::vec2& _leftBottomUV, const glm::vec2& _rightTopUV)
                : leftBottomUV(_leftBottomUV)
                , rightTopUV(_rightTopUV)
            {}

            SubTexture2D()
                : leftBottomUV(0.f)
                , rightTopUV(1.f)
            {}
        };

        Texture2D(const GLuint width, GLuint height, const unsigned char* data,
                  const unsigned int channels = 4, const GLenum filter = GL_LINEAR, const GLenum wrapMode = GL_CLAMP_TO_EDGE);
        ~Texture2D();

        Texture2D() = delete;
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D& operator=(Texture2D&& texture) noexcept;
        Texture2D(Texture2D&& texture) noexcept;

        unsigned int get_width() { return m_width; }
        unsigned int get_height() { return m_height; }

        const SubTexture2D& getSubTexture(const std::string& name) const;
        void addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV);

        void bind() const;

    private:
        unsigned int m_id = 0;
        GLenum m_mode;

        unsigned int m_width = 0;
        unsigned int m_height = 0;

        std::map<std::string, SubTexture2D> m_subTextures;
    };

}