#include "lighting.hpp"

#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, Light)

Light::Light()
{
	aclLightingMgr::lights.push_back(this);
}

void aclLightingMgr::Init()
{

}

LightingSettings aclLightingMgr::GetLightUBO()
{
	LightingSettings settings{};

	settings.lightAmount = lights.size();

	for (int i = 0; i < MAX_LIGHTS; ++i)
	{
		if (lights.size() <= i)
		{
			settings.lightColors[i] = { 1,1,1 };
			settings.lightPositions[i] = {1,1,1};
			settings.lightPowers[i] = 1.0f;
			continue;
		}

		Light light = *lights[i];

		settings.lightColors[i] = { light.Color.r, light.Color.g, light.Color.b };
		settings.lightPositions[i] = light.Entity->Transform.position;
		settings.lightPowers[i] = light.Power;
	}

	settings.roughness = 0.1f;
	settings.metallic = 0.0f;
	settings.albedo = { 1,1,1 };


	return settings;
}
