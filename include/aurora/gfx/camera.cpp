#include "camera.hpp"

#include "renderer.hpp"
#include "agl/agl.hpp"
#include "agl/maths.hpp"
#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, aclCamera)

aclCamera::aclCamera()
{
	if (Main == nullptr)
	{
		Main = this;
	}
}

void aclCamera::Update()
{
	vec2 extent = agl::GetMainFramebufferSize();

	vec3 direction = normalize(Entity->Transform.position - target);

	vec3 cUp = { 0,1,0 };
	right = normalize(cross(cUp, direction));
	up = cross(direction, right);

	view = lookAt(Entity->Transform.position, target, up);

	proj = glm::perspective(glm::radians(fov), flt extent.x / flt extent.y, 0.1f, 10000.0f);
	proj[1][1] *= -1;
}