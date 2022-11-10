#include "Core/Rendering/OpenGL/Texture2D.hpp"

#include <algorithm>
#include <cmath>

namespace rec
{
    Texture2D::Texture2D(const GLuint width, GLuint height, const unsigned char* data,
                         const unsigned int channels, const GLenum filter, const GLenum wrapMode)
        : m_width(width), m_height(height)
    {
        switch (channels)
        {
        case 4:
            m_mode = GL_RGBA;
            break;
        case 3:
            m_mode = GL_RGB;
            break;
        default:
            m_mode = GL_RGBA;
            break;
        }
        /*glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        const GLsizei mip_levels = static_cast<GLsizei>(std::log2(std::max(m_width, m_height))) + 1;
        glTextureStorage2D(m_id, mip_levels, GL_RGB8, m_width, m_height);
        glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glGenerateTextureMipmap(m_id);*/

        glGenTextures(1, &m_id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, m_mode, m_width, m_height, 0, m_mode, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture2D::~Texture2D()
    {
        glDeleteTextures(1, &m_id);
    }

    Texture2D& Texture2D::operator=(Texture2D&& texture) noexcept
    {
        glDeleteTextures(1, &m_id);
        m_id = texture.m_id;
        m_width = texture.m_width;
        m_height = texture.m_height;
        texture.m_id = 0;
        return *this;
    }

    Texture2D::Texture2D(Texture2D&& texture) noexcept
    {
        m_id = texture.m_id;
        m_width = texture.m_width;
        m_height = texture.m_height;
        texture.m_id = 0;
    }

    const Texture2D::SubTexture2D& Texture2D::getSubTexture(const std::string& name) const
    {
        auto it = m_subTextures.find(name);
        if (it != m_subTextures.end())
        {
            return it->second;
        }
        const static SubTexture2D defaultSubTexture;
        return defaultSubTexture;
    }

    void Texture2D::addSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
    {
        m_subTextures.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
    }

    void Texture2D::bind() const
    {
        glBindTexture(GL_TEXTURE_2D, m_id);
    }
}