#if !defined(CAMERA_HPP)
#define CAMERA_HPP

#include "aurora/ecs/component.hpp"
#include "aurora/ecs/entity.hpp"
#include "aurora/utils/utils.hpp"
#include "aurora/math/math.hpp"

class AURORA_API Camera : public Component
{
	CLASS_DECLARATION(Camera)

public:
	Camera(std::string&& initialValue);

	Camera();

	vec3 target = {0,0,0};
	vec3 up = { 0.0,1.0,0.0 };
	vec3 right = { 1.0,0,0 };
	vec3 forward = { 0,0,1 };
	float fov = 45.0f;

	float nearPlane = 0.1f;
	float farPlane = 10000.0f;

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

	void Update() override;

	mat4 GetViewMatrix();

	mat4 GetProjectionMatrix();

	void ModifyTransformationBuffer(TransformationBuffer* bfr);

	inline static Camera* Main = nullptr;

private:
	mat4 view=mat4(1.0), proj=mat4(1.0);
};

#endif // CAMERA_HPP
