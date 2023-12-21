#include "entity.hpp"
Ref<Entity> EntityMgr::CreateEntity(std::string name, Ref<Entity> parent)
{
	Entity entity = Entity();
	entity.Name = name;
	entity.Parent = parent;
	entity.Id = registeredEntities.size();

	Ref entityRef(entity);

	registeredEntities.push_back(entityRef);

	return entityRef;

}

Ref<Entity> EntityMgr::DuplicateEntity(Ref<Entity> e)
{
	Entity entity = Entity();

	entity.Name = e->Name;
	entity.Parent = e->Parent;
	entity.Id = registeredEntities.size();

	entity.Transform.position = e->Transform.position;
	entity.Transform.rotation = e->Transform.rotation;
	entity.Transform.scale = e->Transform.scale;

	for (int i = 0; i < entity.Children.size(); i++)
	{
		auto newC = DuplicateEntity(entity.Children[i]);
		entity.Children[i] = newC;
	}

	for (auto component : e->components)
	{
		auto newComponent = component->clone();

		entity.AttachComponent(newComponent);
	}

	entity.Init();

	Ref entityRef(entity);

	registeredEntities.push_back(entityRef);

	return entityRef;

}

void Entity::Init()
{
	for (auto component : components)
	{
		component->Init();
	}
}

void Entity::Update()
{

	Transform.Update();

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

void Entity::AttachComponent(Ref<Component> component)
{
	component->Entity = this;
	components.push_back(component);
}
