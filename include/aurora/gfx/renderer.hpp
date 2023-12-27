#if !defined(RENDERER_HPP)
#define RENDERER_HPP

#include "lighting.hpp"
#include "agl/agl.hpp"
#include "aurora/ecs/component.hpp"
#include "aurora/utils/utils.hpp"


class AURORA_API MeshRenderer : public Component
{
	CLASS_DECLARATION(MeshRenderer)


public:
	MeshRenderer(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	MeshRenderer();

	void Update() override;

	agl::aglMaterial* material;

	agl::aglMesh* mesh;

	vec3 color=vec3{1};
	float roughness = 1, metallic = 0;

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

private:

	agl::aglUniformBuffer* transformationBuffer = nullptr;
	agl::aglUniformBuffer* lightingBuffer = nullptr;

};


#endif // RENDERER_HPP
