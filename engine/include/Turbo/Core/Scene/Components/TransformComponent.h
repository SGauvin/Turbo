#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Turbo
{
    class TransformComponent
    {
    public:
        TransformComponent() = default;

        glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };

        void setQuat(const glm::quat& quat)
        {
            rotation = glm::eulerAngles(quat);
        }

        glm::quat getQuat() const
        {
            return glm::quat(rotation);
        }

        glm::mat4 getTransform() const
        {
            return glm::translate(glm::mat4(1.f), translation) * glm::toMat4(glm::quat(rotation)) * glm::scale(glm::mat4(1.0f), scale);
        }
    };
} // namespace Turbo
