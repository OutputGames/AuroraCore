#include "lighting.hpp"

#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, Light)

LightingSettings aclLightingMgr::GetLightUBO()
{
	LightingSettings settings{};

	settings.lightAmount = lights.size();

	for (int i = 0; i < lights.size(); ++i)
	{
		Light light = lights[i];

		settings.lightColors[i] = { light.Color.r, light.Color.g, light.Color.b };
		settings.lightPositions[i] = light.Entity->Transform.position;
		settings.lightPowers[i] = light.Power;
	}

	settings.roughness = 0.5;
	settings.metallic = 0.5;
	settings.albedo = { 0.75,0.5,0.25 };


	return settings;
}
