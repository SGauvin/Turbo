#pragma once

#include <glm/gtx/transform.hpp>
#include <Turbo/Core/Input/InputManager.h>

class Camera
{
public:
    void handleInput(const Turbo::InputManager& inputManager)
    {
        glm::vec3 cameraFront = glm::vec3(
            glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
            glm::sin(m_cameraPitch),
            glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
        );

        static const float sensitivity = 0.05f;

        if (inputManager.isKeyDown(Turbo::Keyboard::Key::W))
        {
            m_cameraPosition += cameraFront * sensitivity;
        }
        if (inputManager.isKeyDown(Turbo::Keyboard::Key::S))
        {
            m_cameraPosition -= cameraFront * sensitivity;
        }
        if (inputManager.isKeyDown(Turbo::Keyboard::Key::A))
        {
            m_cameraPosition -= glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
        if (inputManager.isKeyDown(Turbo::Keyboard::Key::D))
        {
            m_cameraPosition += glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
    }

    glm::mat4 getLookAt() const
    {
        glm::vec3 cameraFront = glm::vec3(
            glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
            glm::sin(m_cameraPitch),
            glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
        );
        return glm::lookAt(m_cameraPosition, m_cameraPosition + cameraFront, m_cameraUp);
    }

    const glm::vec3 getPosition() const
    {
        return m_cameraPosition;
    }

    void addYaw(float yawToAdd)
    {
        m_cameraYaw += yawToAdd;
    }

    void addPitch(float pitchToAdd)
    {
        m_cameraPitch += pitchToAdd;
    }
private:
    glm::vec3 m_cameraPosition = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
    float m_cameraYaw = 3.1416 * 1.5;
    float m_cameraPitch = 0;
};
