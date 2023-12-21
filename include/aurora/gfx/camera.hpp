#if !defined(CAMERA_HPP)
#define CAMERA_HPP

#include "aurora/ecs/component.hpp"
#include "aurora/ecs/entity.hpp"
#include "aurora/utils/utils.hpp"
#include "aurora/math/math.hpp"

class MeshRenderer;

class aclCamera : public Component
{
	CLASS_DECLARATION(aclCamera)

public:
	aclCamera(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	aclCamera();

	vec3 target = {0,0,0};
	vec3 up = { 0.0,1.0,0.0 };
	vec3 right = { 1.0,0,0 };
	float fov = 45.0f;

	void Update() override;

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
		bfr->camPos = Entity->Transform.position;
	}

	inline static aclCamera* Main = nullptr;

private:
	mat4 view=mat4(1.0), proj=mat4(1.0);
};

#endif // CAMERA_HPP
