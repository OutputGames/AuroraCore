#include "renderer.hpp"

#include "camera.hpp"
#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, MeshRenderer)

MeshRenderer::MeshRenderer()
{
	material = new agl::aglMaterial;
	mesh = nullptr;
}

void MeshRenderer::Update()
{

	if (!material->shader->setup)
	{
		if (material->shader->GetBindingByName("transformationBuffer") != -1){
			agl::aglUniformBufferSettings settings = {};

			settings.flags = VK_SHADER_STAGE_VERTEX_BIT;

			transformationBuffer = new agl::aglUniformBuffer<TransformationBuffer>(material->shader, settings);

			transformationBuffer->AttachToShader(material->shader, material->shader->GetBindingByName("transformationBuffer"));
		}

		if (material->shader->GetBindingByName("lightingSettings") != -1){
			agl::aglUniformBufferSettings settings = {};

			settings.flags = VK_SHADER_STAGE_FRAGMENT_BIT;

			lightingBuffer = new agl::aglUniformBuffer<LightingSettings>(material->shader, settings);

			lightingBuffer->AttachToShader(material->shader, material->shader->GetBindingByName("lightingSettings"));
		}

		material->shader->Setup();
	}

	TransformationBuffer ubo{};
	Entity->Transform.ModifyTransformationBuffer(&ubo);
	aclCamera::Main->ModifyTransformationBuffer(&ubo);

	transformationBuffer->Update(ubo);
	if (lightingBuffer != nullptr) {

		auto lightUBO = aclLightingMgr::GetLightUBO();

		lightUBO.albedo = color;
		lightUBO.roughness = roughness;
		lightUBO.metallic = metallic;

		lightingBuffer->Update(lightUBO);
	}

	agl::baseSurface->framebuffer->renderPass->renderQueue->AttachQueueEntry({ mesh, material->shader });
}
