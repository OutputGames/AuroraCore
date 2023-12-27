#include "renderer.hpp"

#include "camera.hpp"
#include "agl/agl_ext.hpp"
#include "aurora/ecs/entity.hpp"
#include "aurora/ecs/scene.hpp"

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
			agl::aglBufferSettings settings = {VK_SHADER_STAGE_VERTEX_BIT, sizeof(TransformationBuffer)};

			transformationBuffer = new agl::aglUniformBuffer(material->shader, settings);

			transformationBuffer->AttachToShader(material->shader, material->shader->GetBindingByName("transformationBuffer"));
		}

		if (material->shader->GetBindingByName("lightingSettings") != -1){
			agl::aglBufferSettings settings = {VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(LightingSettings)};


			lightingBuffer = new agl::aglUniformBuffer(material->shader, settings);

			lightingBuffer->AttachToShader(material->shader, material->shader->GetBindingByName("lightingSettings"));
		}

		material->shader->Setup();
	}

	TransformationBuffer ubo{};
	Entity->Transform.ModifyTransformationBuffer(&ubo);
	Camera::Main->ModifyTransformationBuffer(&ubo);

	transformationBuffer->Update(&ubo, sizeof(ubo));
	if (lightingBuffer != nullptr) {

		auto lightUBO = Scene::Current->lightingMgr->GetLightUBO();

		lightUBO.albedo = color;
		lightUBO.roughness = roughness;
		lightUBO.metallic = metallic;

		lightingBuffer->Update(&lightUBO, sizeof(lightUBO));
	}

	agl::GetSurfaceDetails()->framebuffer->renderPass->renderQueue->AttachQueueEntry({ mesh, material->shader });
}

nlohmann::json MeshRenderer::Serialize()
{
	nlohmann::json j = Component::Serialize();

	j["Color"] = MathSerializer::SerializeVec3(color);
	j["Roughness"] = roughness;
	j["Metallic"] = metallic;
	j["Mesh"] = mesh->path;
	j["MeshIndex"] = mesh->meshIndex;
	j["Material"] = {
		{"Shader", material->shader->Serialize()},
	};

	return j;
}

void MeshRenderer::Load(nlohmann::json j)
{
	if (aglPrimitives::GetPrims().size() == 0)
	{
		agl_ext::InstallExtension<aglPrimitives>();
	}

	color = MathSerializer::DeserializeVec3(j["Color"]);

	roughness = j["Roughness"];
	metallic = j["Metallic"];

	const std::vector<std::string> primitivePaths = aglPrimitives::GetPrimNames();

	if (uvector::GetIndexOfElement<std::string>(primitivePaths, j["Mesh"]) == -1)
	{
		mesh = agl::aglMesh::GrabMesh(j["Mesh"], j["MeshIndex"]);
	}
	else
	{
		mesh = aglPrimitives::GetPrims()[static_cast<aglPrimitives::aglPrimitiveType>(uvector::GetIndexOfElement<std::string>(primitivePaths, j["Mesh"]))];
	}

	material->shader = agl::aglShaderFactory::GetShader(j["Material"]["Shader"]["Id"]);

}
