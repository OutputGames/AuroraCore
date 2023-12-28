#include "camera.hpp"

#include "renderer.hpp"
#include "agl/agl.hpp"
#include "agl/maths.hpp"
#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, Camera)

Camera::Camera(std::string&& initialValue): Component(move(initialValue))
{
}

mat4 Camera::GetViewMatrix()
{
	return view;
}

mat4 Camera::GetProjectionMatrix()
{
	return proj;
}

vec3 Camera::GetForwardVector()
{
	const mat4 inverted = glm::inverse(GetViewMatrix());
	const vec3 forward = normalize(glm::vec3(inverted[2]));
	return forward;
}

void Camera::ModifyTransformationBuffer(TransformationBuffer* bfr)
{
	bfr->view = GetViewMatrix();
	bfr->proj = GetProjectionMatrix();
	bfr->camPos = vec4(Entity->Transform.position, 1.0);
}

Camera::Camera()
{
	Main = this;
}

nlohmann::json Camera::Serialize()
{
	nlohmann::json j = Component::Serialize();

	j["Target"] = MathSerializer::SerializeVec3(target);
	j["Up"] = MathSerializer::SerializeVec3(up);
	j["Right"] = MathSerializer::SerializeVec3(right);

	j["FOV"] = fov;
	j["NearPlane"] = nearPlane;
	j["FarPlane"] = farPlane;

	return j;
}

void Camera::Load(nlohmann::json j)
{
	target = MathSerializer::DeserializeVec3(j["Target"]);
	up = MathSerializer::DeserializeVec3(j["Up"]);
	right = MathSerializer::DeserializeVec3(j["Right"]);

	fov = j["FOV"];
	nearPlane = j["NearPlane"];
	farPlane = j["FarPlane"];
}

void Camera::Update()
{
	vec2 extent = agl::GetMainFramebufferSize();

	vec3 direction = normalize(Entity->Transform.position - target);

	vec3 cUp = { 0,1,0 };
	right = normalize(cross(cUp, direction));
	up = cross(direction, right);

	view = lookAt(Entity->Transform.position, target, up);


	if (extent.x != 0 && extent.y != 0) {
		proj = glm::perspective(glm::radians(fov), flt extent.x / flt extent.y, nearPlane, farPlane);
		proj[1][1] *= -1;
	}

	mat4 inverted = glm::inverse(view);
	forward = normalize(glm::vec3(inverted[2]));
}
