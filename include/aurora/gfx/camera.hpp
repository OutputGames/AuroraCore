#if !defined(CAMERA_HPP)
#define CAMERA_HPP

#include "aurora/utils/utils.hpp"
#include "aurora/math/math.hpp"

struct Camera
{
	vec3 position={0,0,2};
	vec3 target;
	vec3 up = { 0.0,1.0,0.0 };
	vec3 right = { 1.0,0,0 };
	float fov = 45.0f;

	void Update(vec2 extents);

	mat4 GetViewMatrix()
	{
		return view;
	}

	mat4 GetProjectionMatrix()
	{
		return proj;
	}

	void ModifyTransformationBuffer(TransformationBuffer* bfr)
	{
		bfr->view = GetViewMatrix();
		bfr->proj = GetProjectionMatrix();
		bfr->camPos = position;
	}

private:
	mat4 view=mat4(1.0), proj=mat4(1.0);
};

#endif // CAMERA_HPP
