#include "entity.hpp"

#include <xutility>

#include "json.hpp"
#include "registry.hpp"
#include "scene.hpp"

std::map<std::string, Ref<Component>(*)()> ComponentRegistry::cmp_map = std::map<std::string, Ref<Component>(*)()>();

Ref<Entity> EntityMgr::CreateEntity(std::string name, Ref<Entity> parent)
{
	Entity entity = {};
	entity.Name = std::move(name);
	entity.SetParent(parent);

	entity.Id = (u32)registeredEntities.size();

	Ref entityRef(entity);

	entity.RefPtr = entityRef;

	registeredEntities.push_back(entityRef);

	return entityRef;

}

Ref<Entity> EntityMgr::DuplicateEntity(Ref<Entity> e)
{
	Entity entity = {};

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

Ref<Entity> EntityMgr::GetEntity(std::string name)
{
	for (auto registered_entity : registeredEntities)
	{
		if (registered_entity->Name == name)
		{
			return registered_entity;
		}
	}

	return nullptr;
}

nlohmann::json EntityMgr::Serialize() const
{
	nlohmann::json j;

	for (auto entity : registeredEntities)
	{
		if (entity->Parent.Get() == nullptr) {
			j["Entities"][entity->Id] = entity->Serialize();
		}
	}

	return j;
}

void EntityMgr::Load(nlohmann::json j)
{
	for (auto ej : j["Entities"])
	{
		if (ej.contains("Parent"))
			continue;
		Ref entity = CreateEntity(ej["Name"]);
		entity->Load(ej);
		registeredEntities.push_back(entity);
	}

	for (auto registered_entity : registeredEntities)
	{
		registered_entity->Init();
	}
}

void Entity::Init() const
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

void Entity::AddChild(Ref<Entity> Child)
{

	Child->Parent = RefPtr;

	Children.push_back(Child);
}

void Entity::SetParent(Ref<Entity> p)
{
	if (p.Get() != nullptr) {
		p->AddChild(this);
	}
}

void Entity::AttachComponent(Ref<Component> component)
{
	component->Entity = this;
	components.push_back(component);
}

Ref<Component> Entity::AttachComponent(std::string name)
{
	auto c = ComponentRegistry::cmp_map[name]();
	AttachComponent(c);
	return c;
}

void Entity::OnCollisionEnter(Collision* other) const
{
	for (auto component : components)
	{
		component->OnCollisionEnter(other);
	}
}

void Entity::OnCollisionExit(Collider* other) const
{
	for (auto component : components)
	{
		component->OnCollisionExit(other);
	}
}

void Entity::OnCollisionStay(Collision* other) const
{
	for (auto component : components)
	{
		component->OnCollisionStay(other);
	}
}

nlohmann::json Entity::Serialize()
{
	nlohmann::json j;

	j["Id"] = Id;
	j["Name"] = Name;
	j["Enabled"] = Enabled;

	j["Transform"] = Transform.Serialize();

	j["Children"] = {};

	for (int i = 0; i < Children.size(); ++i)
	{
		Ref<Entity> child = Children[i];
		j["Children"][i] = child->Serialize();
	}

	if (Parent.Get() != nullptr) {
		j["Parent"] = Parent->Id;
	}

	j["Components"] = {};

	for (int i = 0; i < components.size(); ++i)
	{
		j["Components"].push_back(components[i]->Serialize());
	}

	return j;
}

void Entity::Load(nlohmann::json j)
{
	
	Name = j["Name"];
	Enabled = j["Enabled"];
	Transform.Load(j["Transform"]);

	for (auto cj : j["Children"])
	{
		Ref<Entity> Child = Scene::Current->entityMgr->CreateEntity(cj["Name"], RefPtr);
		Child->Load(cj);
	}

	for (auto cj : j["Components"])
	{
		auto c = AttachComponent(cj["Name"]);
		c->Load(cj);
	}
	
}

Entity::Entity()
{
	Transform = {};
}