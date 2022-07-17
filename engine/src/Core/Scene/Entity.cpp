#include "Turbo/Core/Scene/Entity.h"
#include "Turbo/Core/Scene/Scene.h"

namespace Turbo
{
    Entity::Entity()
        : m_entity(entt::null)
        , m_scene(nullptr)
    {
    }

    Entity::Entity(entt::entity entity, Scene* scene)
        : m_entity(entity)
        , m_scene(scene)
    {
    }

    Entity::operator entt::entity() const
    {
        return m_entity;
    }
} // namespace Turbo