#if !defined(MATH_HPP)
#define MATH_HPP

#include <aurora/utils/utils.hpp>

#include "json.hpp"
#include "glm/gtx/quaternion.hpp"

struct MathSerializer
{
	static nlohmann::json SerializeVec3(vec3 v)
	{
		nlohmann::json j;

		j = {v.x,v.y,v.z};

		return j;
	}

	static nlohmann::json SerializeVec2(vec2 v)
	{
		nlohmann::json j;

		j = { v.x,v.y };

		return j;
	}

	static nlohmann::json SerializeVec4(vec4 v)
	{
		nlohmann::json j;

		j = { v.x,v.y,v.z, v.w };

		return j;
	}

	static vec3 DeserializeVec3(nlohmann::json j)
	{
		return {j[0], j[1], j[2]};
	}

	static vec4 DeserializeVec4(nlohmann::json j)
	{
		return { j[0], j[1], j[2], j[3]};
	}
};

struct AURORA_API TransformationBuffer
{
	alignas(16) mat4 model;
	alignas(16) mat4 view;
	alignas(16) mat4 proj;
	alignas(16) vec4 camPos;
};

struct AURORA_API Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	nlohmann::json Serialize();
	void Load(nlohmann::json j);

public:


	Transform(nullptr_t): position(0,0,0), rotation(0,0,0), scale(1,1,1)
	{
	}

	Transform() : position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1)
	{
	}

	void Update()
	{
		
	}

	void ModifyTransformationBuffer(TransformationBuffer* bfr)
	{
		mat4 model(1.0);

		model = translate(model, position);

		model = glm::rotate(model, radians(rotation.x), vec3{ 1,0,0 });
		model = glm::rotate(model, radians(rotation.y), vec3{ 0,1,0 });
		model = glm::rotate(model, radians(rotation.z), vec3{ 0,0,1 }); 
		
		model = glm::scale(model, scale);

		mat3 normal = transpose(inverse(mat3(model)));

		//normal[1][1] *= -1;

		bfr->model = model;
		//bfr->normalMatrix = normal;

	}
};

inline nlohmann::json Transform::Serialize()
{
	nlohmann::json j;

	j["Position"] = MathSerializer::SerializeVec3(position);
	j["Rotation"] = MathSerializer::SerializeVec3(rotation);
	j["Scale"] = MathSerializer::SerializeVec3(scale);

	return j;
}

inline void Transform::Load(nlohmann::json j)
{
	position = { j["Position"][0], j["Position"][1], j["Position"][2] };
	rotation = { j["Rotation"][0], j["Rotation"][1], j["Rotation"][2] };
	scale = { j["Scale"][0], j["Scale"][1], j["Scale"][2] };
}

struct AURORA_API aclMath
{
	static vec3 resolveZYXToXYZ(vec3 v)
	{
		return { -v.z, v.y, v.x };
	}

	static float random(float min, float max)
	{

		return  min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
	}
};

#endif // MATH_HPP
