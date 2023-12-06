#include "renderer.hpp"

CLASS_DEFINITION(Component, MeshRenderer)

MeshRenderer::MeshRenderer()
{
	material = new agl::aglMaterial;
	mesh = nullptr;
}

void MeshRenderer::Update()
{
	agl::baseSurface->framebuffer->renderPass->renderQueue->AttachQueueEntry({ mesh, material->shader });
}
