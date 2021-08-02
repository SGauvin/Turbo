#pragma once

#include <cmath>
#include <glm/gtx/transform.hpp>
#include <limits>
#include <Turbo/Core/Input/InputManager.h>

class Camera
{
public:
    Camera(Turbo::InputManager& inputManager)
        : m_inputContext(inputManager.createInputContext())
    {
        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isForwardKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::W);

        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isBackwardKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::S);

        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isLeftKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::A);

        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isRightKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::D);

        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isUpKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::LShift);

        m_inputContext->bindKeyToState([this](bool keyState)
        {
            m_isDownKeyDown = keyState;
            return true;
        }, Turbo::Keyboard::Key::LControl);

        m_inputContext->bindMouseMoveEvents([this](const Turbo::Mouse::MoveEvent& moveEvent)
        {
            static constexpr float sens = 1.f / 1000.f;
            addPitch(-moveEvent.movement.y * sens);
            addYaw(moveEvent.movement.x * sens);
            return true;
        });
    }

    void update()
    {
        glm::vec3 cameraFront = glm::vec3(
            glm::cos(m_cameraYaw) * glm::cos(m_cameraPitch),
            glm::sin(m_cameraPitch),
            glm::sin(m_cameraYaw) * glm::cos(m_cameraPitch)
        );

        static const float sensitivity = 0.02f;

        if (m_isForwardKeyDown && !m_isBackwardKeyDown)
        {
            m_cameraPosition += cameraFront * sensitivity;
        }
        if (m_isBackwardKeyDown && !m_isForwardKeyDown)
        {
            m_cameraPosition -= cameraFront * sensitivity;
        }
        if (m_isLeftKeyDown && !m_isRightKeyDown)
        {
            m_cameraPosition -= glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
        if (m_isRightKeyDown && !m_isLeftKeyDown)
        {
            m_cameraPosition += glm::normalize(glm::cross(cameraFront, m_cameraUp)) * sensitivity;
        }
        if (m_isUpKeyDown && !m_isDownKeyDown)
        {
            m_cameraPosition += m_cameraUp * sensitivity;
        }
        if (m_isDownKeyDown && !m_isUpKeyDown)
        {
            m_cameraPosition -= m_cameraUp * sensitivity;
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
        static constexpr float maxAngle = M_PI / 2.f - 5 * std::numeric_limits<float>::epsilon();
        m_cameraPitch = std::max(-maxAngle, std::min(maxAngle, m_cameraPitch));
    }
private:
    Turbo::InputContext* m_inputContext;
    bool m_isForwardKeyDown = false;
    bool m_isBackwardKeyDown = false;
    bool m_isLeftKeyDown = false;
    bool m_isRightKeyDown = false;
    bool m_isUpKeyDown = false;
    bool m_isDownKeyDown = false;

    glm::vec3 m_cameraPosition = glm::vec3(0.f, 0.f, 3.f);
    glm::vec3 m_cameraUp = glm::vec3(0.f, 1.f, 0.f);
    float m_cameraYaw = 3.1416 * 1.5;
    float m_cameraPitch = 0;
};
