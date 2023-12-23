#if !defined(MATH_HPP)
#define MATH_HPP

#include <aurora/utils/utils.hpp>

#include "glm/gtx/quaternion.hpp"


struct AURORA_API TransformationBuffer
{
	alignas(16) mat4 model;
	alignas(16) mat4 view;
	alignas(16) mat4 proj;
	alignas(16) mat3 normalMatrix;
	alignas(16) vec3 camPos;
};

struct AURORA_API Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

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

		bfr->model = model;
		bfr->normalMatrix = normal;

	}
};

struct AURORA_API aclMath
{
	static vec3 resolveZYXToXYZ(vec3 v)
	{
		return { -v.z, v.y, v.x };
	}

	static float random01()
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}
};

#endif // MATH_HPP
