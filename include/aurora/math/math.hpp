#if !defined(MATH_HPP)
#define MATH_HPP

#include <aurora/utils/utils.hpp>

#include "glm/gtx/quaternion.hpp"


struct TransformationBuffer
{
	alignas(16) mat4 model;
	alignas(16) mat4 view;
	alignas(16) mat4 proj;
	alignas(16) mat3 normalMatrix;
	alignas(16) vec3 camPos;
};

struct Transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	Transform(nullptr_t): position(0,0,0), rotation(0,0,0,0), scale(1,1,1)
	{
	}

	Transform() : position(0, 0, 0), rotation(0, 0, 0,0), scale(1, 1, 1)
	{
	}

	void ModifyTransformationBuffer(TransformationBuffer* bfr)
	{
		mat4 model(1.0);

		model = translate(model, position);
		model *= toMat4(rotation);
		model = glm::scale(model, scale);

		mat3 normal = transpose(inverse(mat3(model)));

		bfr->model = model;
		bfr->normalMatrix = normal;

	}
};


#endif // MATH_HPP
