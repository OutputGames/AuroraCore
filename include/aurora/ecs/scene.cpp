#include "scene.hpp"

#include "json.hpp"
#include "agl/agl.hpp"
#include "aurora/physics/physics.hpp"
#include "aurora/utils/fs.hpp"

Scene* Scene::Load(std::string filename, SceneCreationSettings settings)
{

	Scene* scene = Create(settings);

	nlohmann::json j = nlohmann::json::parse(ReadString(filename));

	scene->Name = j["Name"];

	agl::aglShaderFactory::SetData(j["ShaderFactory"]);
	agl::aglTextureFactory::SetData(j["TextureFactory"]);

	agl::aglShaderFactory::Load(j["ShaderFactory"]);
	agl::aglTextureFactory::Load(j["TextureFactory"]);

	scene->entityMgr->Load(j["EntityMgr"]);

	return scene;
}

Scene* Scene::Create(SceneCreationSettings settings)
{
	Scene* scene = new Scene;
	scene->Name = settings.Name;

	if (settings.UseDefaultScene)
	{
		
	}

	if (Current == nullptr || settings.SetAsCurrent)
	{
		Current = scene;
	}

	aclPhysicsMgr::Destroy();

	return scene;
}

void Scene::Update()
{
	entityMgr->UpdateAllEntities();
}

void Scene::Serialize(std::string path)
{
	nlohmann::json j;

	j["Name"] = Name;

	j["EntityMgr"] = entityMgr->Serialize();
	j["ShaderFactory"] = agl::aglShaderFactory::Serialize();
	j["TextureFactory"] = agl::aglTextureFactory::Serialize();

	std::string data = j.dump(JSON_INDENT_AMOUNT);

	WriteString(path, data);
}

Scene::Scene()
{
	entityMgr = new EntityMgr;
	lightingMgr = new LightingMgr;

	lightingMgr->Init();
}
