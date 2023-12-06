#if !defined(RENDERER_HPP)
#define RENDERER_HPP

#include "agl/agl.hpp"
#include "aurora/ecs/component.hpp"
#include "aurora/utils/utils.hpp"


class MeshRenderer : public Component
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

private:



};


#endif // RENDERER_HPP
