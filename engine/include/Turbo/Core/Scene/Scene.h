#ifndef INCLUDED_TURBO_SCENE_H
#define INCLUDED_TURBO_SCENE_H

#include <entt/entt.hpp>

namespace Turbo
{
    class Entity;

    class Scene
    {
    public:
        Scene();
        
        void update();
        void draw(float lag = 1.0f);

        Entity createEntity();
        void deleteEntity(const Entity& entity);

    private:
        entt::registry m_registry;

        friend class Entity;
    };
} // Turbo

#endif // INCLUDED_TURBO_SCENE_H
