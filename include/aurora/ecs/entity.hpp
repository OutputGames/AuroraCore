#if !defined(ENTITY_HPP)
#define ENTITY_HPP

#include "component.hpp"
#include "json.hpp"
#include "aurora/math/math.hpp"
#include "aurora/utils/utils.hpp"

class Collider;
struct Collision;

struct AURORA_API Entity
{

    u32 Id=-1;
    std::string Name="Uninitialized";
    bool Enabled=true;
    Transform Transform;

    std::vector<Ref<Entity>> Children;
    Ref<Entity> Parent = nullptr;

    void Init() const;
    void Update();
    void Destroy();

    void AddChild(Ref<Entity> Child);

    void SetParent(Ref<Entity> Parent);

    std::vector<Ref<Component>> components;

    template <typename T, typename... Args>
    T* AttachComponent(Args&&... params);

    void AttachComponent(Ref<Component> component);

    Ref<Component> AttachComponent(std::string name);

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

    friend struct EntityMgr;

    void OnCollisionEnter(Collision* other) const;
    void OnCollisionExit(Collider* other) const;
    void OnCollisionStay(Collision* other) const;

    nlohmann::json Serialize();
    void Load(nlohmann::json j);

private:

    Ref<Entity> RefPtr;

    Entity();
};

template <typename T, typename ... Args>
T* Entity::AttachComponent(Args&&... params)
{
    if (!GetComponent<T>()) {
        components.emplace_back(std::make_shared< T >(forward< Args >(params)...));
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

struct AURORA_API EntityMgr
{
	std::vector<Ref<Entity>> registeredEntities;

    Ref<Entity> CreateEntity(std::string name="", Ref<Entity> parent=nullptr);
    Ref<Entity> DuplicateEntity(Ref<Entity> e);
    Ref<Entity> GetEntity(std::string name);

    void UpdateAllEntities() const
    {
	    for (auto registered_entity : registeredEntities)
	    {
            registered_entity->Update();
	    }
    }

    nlohmann::json Serialize() const;
    void Load(nlohmann::json j);

};

#endif // ENTITY_HPP
