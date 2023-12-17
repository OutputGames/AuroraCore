#include <iomanip>

#include "aal/aal.hpp"
#include "aurora/aurora.hpp"

#include "agl/agl.hpp"
#include "agl/agl_ext.hpp"
#include "agl/ext.hpp"
#include "agl/maths.hpp"
#include "agl/re.hpp"
#include "aurora/ecs/audio.hpp"
#include "aurora/gfx/camera.hpp"
#include "aurora/gfx/renderer.hpp"

int main(int argc, char* argv[])
{

	agl_details* details = new agl_details;

	details->applicationName = "agl testing";
	details->engineName = "AuroraEngine";
#ifdef GRAPHICS_VULKAN
	details->engineVersion = VK_MAKE_VERSION(1, 0, 0);
	details->applicationVersion = VK_MAKE_VERSION(1, 0, 0);

#endif
	details->Width = 800;
	details->Height = 600;

	SDLUtils::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);

	agl::agl_init(details);
	agl::complete_init();

	//agl_ext::InstallExtension<aglImGuiExtension>(new aglImGuiExtension());

	EntityMgr entityMgr;

	Ref<Entity> entity = entityMgr.CreateEntity("NewEntity");

	//AudioPlayer* audioPlayer = entity->AttachComponent<AudioPlayer>();

	//audioPlayer->SetAudio(new Sound(new SoundCreateInfo{ Sound::LoadSoundBuffer("testresources/news.wav") }));

	float pos = 100;

	Camera* camera = new Camera;

	agl_ext::InstallExtension<aglPrimitives>(new aglPrimitives);

	agl::aglTexture* equiTex = new agl::aglTexture("testresources/textures/test.hdr", VK_FORMAT_R8G8B8A8_SRGB);

	agl::aglUniformBuffer<TransformationBuffer>* camBuffer;

	agl::aglShader* shader = new agl::aglShader(new agl::aglShader::aglShaderSettings{ "testresources/shaders/flat3d/main.vert","testresources/shaders/flat3d/main.frag", VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_COMPARE_OP_LESS_OR_EQUAL });

	agl::aglUniformBufferSettings settings = {};

	settings.flags = VK_SHADER_STAGE_VERTEX_BIT;

	camBuffer = new agl::aglUniformBuffer<TransformationBuffer>(shader, settings);

	camBuffer->AttachToShader(shader, shader->GetBindingByName("ubo"));

	agl::aglShader* brdfshader = new agl::aglShader(new agl::aglShader::aglShaderSettings{ "testresources/shaders/equi/main.vert","testresources/shaders/equi/main.frag", VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_COMPARE_OP_ALWAYS });

	agl::aglTexture* tex = new agl::aglTexture(brdfshader, agl::aglTextureCreationInfo{512,512,equiTex->channels,true, equiTex});

	shader->AttachTexture(tex, shader->GetBindingByName("tex"));

	shader->Setup();

	MeshRenderer* renderer = entity->AttachComponent<MeshRenderer>();
	renderer->material->shader = shader;
	renderer->mesh = aglPrimitives::prims[aglPrimitives::CUBE];

	bool done = false;
	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(agl::window))
				done = true;
		}
		agl::event = &event;

		agl_ext::Refresh();

		pos -= 0.0001f;

		entityMgr.UpdateAllEntities();

		float radius = 2;

		camera->position = { 0,0,0};
		camera->target = { sinf(pos) * radius,0,cosf(pos) * radius };
		camera->fov = 90.0f;

		camera->Update(aglMath::ConvertExtents(agl::baseSurface->framebuffer->extent));

		TransformationBuffer ubo{};

		entity->Transform.ModifyTransformationBuffer(&ubo);
		camera->ModifyTransformationBuffer(&ubo);

		ubo.proj[1][1] *= -1;

		camBuffer->Update(ubo);

		agl::record_command_buffer(agl::currentFrame);

		agl::baseSurface->framebuffer->renderPass->End(agl::baseSurface->commandBuffer->GetCommandBuffer(agl::currentFrame ));
		agl::FinishRecordingCommandBuffer(agl::currentFrame);

		//agl_ext::installedExtensions[AGL_EXTENSION_IMGUI_LAYER_NAME]->Refresh();

		agl::UpdateFrame();
	}

	entity->Destroy();

	agl_ext::UninstallAll();
	agl::Destroy();

	return 0;

}
