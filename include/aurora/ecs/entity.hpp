#if !defined(ENTITY_HPP)
#define ENTITY_HPP

#include "component.hpp"
#include "aurora/math/math.hpp"
#include "aurora/utils/utils.hpp"

using namespace std;

struct Entity
{

    u32 Id=-1;
    std::string Name="Uninitialized";
    bool Enabled=true;
    Transform Transform;

    std::vector<Ref<Entity>> Children;
    Ref<Entity> Parent;

    void Update();
    void Destroy();

    void AddChild(Entity Child);

    vector<Ref<Component>> components;

    template <typename T, typename... Args>
    T* AttachComponent(Args&&... params);

    template <typename T>
    T* GetComponent()
    {
	    for (auto component : components)
	    {
		    if (component->IsClassType(T::Type))
		    {
                return static_cast<T*>(component.Get());
		    }
	    }

        return static_cast<T*>(nullptr);
    }

private:

    friend struct EntityMgr;

};

template <typename T, typename ... Args>
T* Entity::AttachComponent(Args&&... params)
{
    if (!GetComponent<T>()) {
        components.emplace_back(make_shared< T >(forward< Args >(params)...));
        for (auto&& component : components) {
            if (component->IsClassType(T::Type)) {
                T* comp = static_cast<T*>(component.Get());
                comp->Entity = this;
                return comp;
            }
        }
    }

    return static_cast<T*>(nullptr);
}

struct EntityMgr
{
    vector<Ref<Entity>> registeredEntities;

    Ref<Entity> CreateEntity(string name="", Ref<Entity> parent=nullptr);

    void UpdateAllEntities() const
    {
	    for (auto registered_entity : registeredEntities)
	    {
            registered_entity->Update();
	    }
    }

};

#endif // ENTITY_HPP
