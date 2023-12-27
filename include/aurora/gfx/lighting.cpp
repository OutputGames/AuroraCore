#include "lighting.hpp"

#include "aurora/ecs/entity.hpp"
#include "aurora/ecs/scene.hpp"

CLASS_DEFINITION(Component, Light)

Light::Light()
{
	Scene::Current->lightingMgr->lights.push_back(this);
}

nlohmann::json Light::Serialize()
{
	nlohmann::json j = Component::Serialize();

	j["Color"] = MathSerializer::SerializeVec4(Color);
	j["Power"] = Power;

	return j;
}

void Light::Load(nlohmann::json j)
{
	Power = j["Power"];
	Color = MathSerializer::DeserializeVec4(j["Color"]);
}

void LightingMgr::Init()
{

}


LightingSettings LightingMgr::GetLightUBO()
{
	LightingSettings settings{};

	settings.lightAmount = lights.size();

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (lights.size() <= i)
		{
			settings.lightColors[i] = vec4{ 1,1,1,1 };
			settings.lightPositions[i] = vec4{1,1,1,1};
			settings.lightPowers[i] = 1.0f;
			continue;
		}

		Light light = *lights[i];

		settings.lightColors[i] = { light.Color.r, light.Color.g, light.Color.b,0 };
		settings.lightPositions[i] = make_vec4(light.Entity->Transform.position);
		settings.lightPowers[i] = light.Power;
	}


	return settings;
}
