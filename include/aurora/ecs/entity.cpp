#include "entity.hpp"
Ref<Entity> EntityMgr::CreateEntity(string name, Ref<Entity> parent)
{
	Entity entity = Entity();
	entity.Name = name;
	entity.Parent = parent;
	entity.Id = registeredEntities.size();

	Ref entityRef(entity);

	registeredEntities.push_back(entityRef);

	return entityRef;

}

void Entity::Update()
{
for (auto component : components)
{
	component->Update();
}
}

void Entity::Destroy()
{
}

void Entity::AddChild(Entity Child)
{
	const Ref<Entity> c_ptr(Child);

	Children.push_back(c_ptr);
}
