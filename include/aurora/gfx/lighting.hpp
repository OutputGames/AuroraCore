#if !defined(LIGHTING_HPP)
#define LIGHTING_HPP

#include "aurora/ecs/component.hpp"
#include "aurora/types/color.hpp"
#include "aurora/utils/utils.hpp"

const int MAX_LIGHTS=16;

struct LightingSettings
{
	alignas(16) vec3 lightPositions[MAX_LIGHTS];
	alignas(16) vec3 lightColors[MAX_LIGHTS];
	alignas(16) float lightPowers[MAX_LIGHTS];
	alignas(4) int lightAmount;

	alignas(16) vec3 albedo;
	alignas(4) float metallic;
	alignas(4) float roughness;
};


class Light : public Component
{
	CLASS_DECLARATION(Light)

public:
	Light(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	Light() = default;

	Color Color = {1,1,1};
	float Power;
};

struct aclLightingMgr
{
    std::vector<Light> lights;
	void init();


	LightingSettings GetLightUBO();
};

#endif // LIGHTING_HPP
