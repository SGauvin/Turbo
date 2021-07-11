#include "Turbo/Core/Scene/Scene.h"
#include "Turbo/Core/Scene/Entity.h"

namespace Turbo
{
    Scene::Scene()
    {}
        
    void Scene::update()
    {}

    void Scene::draw(float lag)
    {}

    Entity Scene::createEntity()
    {
        return Entity(m_registry.create(), this);
    }
    
    void Scene::deleteEntity(const Entity& entity)
    {
        m_registry.destroy(entity);
    }
} // namespace Turbo