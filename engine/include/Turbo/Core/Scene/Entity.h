#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include "Turbo/Core/Scene/Scene.h"

namespace Turbo
{
    class Entity
    {
    public:
        Entity();
        Entity(entt::entity entity, Scene* scene);

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            T& component = m_scene->m_registry.emplace<T>(m_entity, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        void removeComponent()
        {
            m_scene->m_registry.remove<T>(m_entity);
        }

        template<typename T>
        bool hasComponent()
        {
            return m_scene->m_registry.all_of<T>(m_entity);
        }

        template<typename T>
        T& getComponent()
        {
            return m_scene->m_registry.get<T>(m_entity);
        }

        operator entt::entity() const;

    private:
        entt::entity m_entity;
        Scene* m_scene;
    };
} // namespace Turbo
