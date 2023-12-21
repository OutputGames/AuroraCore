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
#include "aurora/gfx/lighting.hpp"
#include "aurora/gfx/renderer.hpp"
#include "aurora/input/input.hpp"
#include "aurora/physics/physics.hpp"
#include "glm/gtx/euler_angles.hpp"

int main(int argc, char* argv[])
{

	agl_details* details = new agl_details;

	details->applicationName = "agl testing";
	details->engineName = "AuroraEngine";
	details->engineVersion = VK_MAKE_VERSION(1, 0, 0);
	details->applicationVersion = VK_MAKE_VERSION(1, 0, 0);

	details->Width = 800;
	details->Height = 600;

	SDLUtils::Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);

	aclPhysicsMgr::Setup();

	agl::agl_init(details);
	agl::complete_init();

	//agl_ext::InstallExtension<aglImGuiExtension>(new aglImGuiExtension());

	EntityMgr entityMgr;

	Ref<Entity> cameraEntity = entityMgr.CreateEntity("Camera");
	Ref<Entity> entity = entityMgr.CreateEntity("NewEntity");

	//AudioPlayer* audioPlayer = entity->AttachComponent<AudioPlayer>();

	//audioPlayer->SetAudio(new Sound(new SoundCreateInfo{ Sound::LoadSoundBuffer("testresources/news.wav") }));

	float pos = 100;


	aclLightingMgr::Init();

	Ref<Entity> lightEntity = entityMgr.CreateEntity("Light");
	Light* light = lightEntity->AttachComponent<Light>();
	light->Color = Color{ 1,1,1 };
	light->Power = 1;

	lightEntity->Transform.position = { 3,3,3 };
	light->Power = 10.0f;

	aclCamera* camera = cameraEntity->AttachComponent<aclCamera>();

	camera->target = vec3{ 0 };
	camera->fov = 90.0f;

	cameraEntity->Transform.position = { 1,1,1 };
	agl_ext::InstallExtension<aglPrimitives>(new aglPrimitives);

	agl::aglTexture* equiTex = new agl::aglTexture("testresources/textures/test.hdr", VK_FORMAT_R8G8B8A8_SRGB);
	agl::aglShader* brdfshader = new agl::aglShader({ "testresources/shaders/equi/main.vert","testresources/shaders/equi/main.frag", VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_COMPARE_OP_ALWAYS });
	agl::aglTexture* tex = new agl::aglTexture(brdfshader, agl::aglTextureCreationInfo{512,512,equiTex->channels,true, equiTex});

	agl::aglShader* shader = new agl::aglShader({ "testresources/shaders/cubemap/main.vert","testresources/shaders/cubemap/main.frag", VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_COMPARE_OP_LESS_OR_EQUAL });
	shader->AttachTexture(tex, shader->GetBindingByName("tex"));
	MeshRenderer* renderer = entity->AttachComponent<MeshRenderer>();
	renderer->material->shader = shader;
	renderer->mesh = aglPrimitives::prims[aglPrimitives::CUBE];

	Ref<Entity> plane = entityMgr.CreateEntity("Plane");
	plane->Transform.scale = { 100,1,100 };
	//plane->Transform.rotation = vec3{ 45,0,0 };
	MeshRenderer* planeRenderer = plane->AttachComponent<MeshRenderer>();
	agl::aglShader* planeShader = new agl::aglShader({ "testresources/shaders/fresnel/main.vert", "testresources/shaders/fresnel/main.frag", VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE });
	planeRenderer->material->shader = planeShader;
	planeRenderer->mesh = aglPrimitives::prims[aglPrimitives::CUBE];
	auto planeCollider = plane->AttachComponent<BoxCollider>();
	planeCollider->Extents = { 100,1,100 };
	Rigidbody* planeBody = plane->AttachComponent<Rigidbody>();
	planeBody->motionType = Rigidbody::STATIC;
	planeBody->Init();

	int it = 3;
	int amt = 5;



	for (int x = 0; x < amt*it; x+=it)
	{
		for (int y = 0; y < amt*it; y+=it)
		{
			for (int z = 0; z < amt*it; z+=it)
			{
				agl::aglShader* sphereShader = new agl::aglShader({ "testresources/shaders/fresnel/main.vert", "testresources/shaders/fresnel/main.frag", VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE });
				Ref<Entity> sphere = entityMgr.CreateEntity("Sphere");
				MeshRenderer* sphereRenderer = sphere->AttachComponent<MeshRenderer>();
				sphereRenderer->material->shader = sphereShader;
				sphereRenderer->mesh = aglPrimitives::prims[aglPrimitives::SPHERE];
				sphere->Transform.position = vec3{ x,10+y,z };
				auto sphereCollider = sphere->AttachComponent<SphereCollider>();
				Rigidbody* sphereBody = sphere->AttachComponent<Rigidbody>();
				sphereBody->Init();
				sphereBody->SetRestitution(1);

				sphereRenderer->color = { aclMath::random01(),aclMath::random01(),aclMath::random01() };
				sphereRenderer->roughness = 0.1f;
				sphereRenderer->metallic = 1;
			}
		}
	}

	aclPhysicsMgr::system->OptimizeBroadPhase();

	aim::aimGamepad* gamepad =
		aim::CreateGamepad(0);

	float posY = 1.0f;

	float radius = 9;

	while (!agl::closeWindow)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			agl::PollEvent(event);
			aim::PollEvents(&event);
		}
		agl::event = &event;

		if (gamepad->triggerAxis.y > 0)
			gamepad->Rumble(abs(sinf(pos)), 10);

		if (pos < -4000)
		{
			pos = 0;
		}

		if (posY < -4000)
		{
			posY = 1;
		}

		if (radius < -4000)
		{
			radius = 3;
		}

		agl_ext::Refresh();

		if (gamepad->IsButtonPressed(SDL_CONTROLLER_BUTTON_DPAD_UP)) {
			agl::aglShaderFactory::ReloadAllShaders();
		}

		if (gamepad->IsButtonPressed(SDL_CONTROLLER_BUTTON_X)) {
			for (auto registered_entity : entityMgr.registeredEntities)
			{
				if (registered_entity->Name == "Sphere")
				{
					registered_entity->GetComponent<Rigidbody>()->Reset();
				}
			}
		}

		pos += gamepad->leftStickAxis.x * 0.05f;
		posY += gamepad->leftStickAxis.y * 0.05f;

		posY = std::clamp(posY, -1.0f, 1.0f);

		radius += gamepad->rightStickAxis.y * 0.5f;

		camera->Entity->Transform.position = vec3{ sinf(pos) * radius, posY * radius, cosf(pos) * radius};


		planeRenderer->color = { 0.1,0.1,1 };

		aclPhysicsMgr::Update(agl::deltaTime);
		entityMgr.UpdateAllEntities();

		agl::record_command_buffer(agl::currentFrame);

		agl::baseSurface->framebuffer->renderPass->End(agl::baseSurface->commandBuffer->GetCommandBuffer(agl::currentFrame ));
		agl::FinishRecordingCommandBuffer(agl::currentFrame);

		//agl_ext::installedExtensions[AGL_EXTENSION_IMGUI_LAYER_NAME]->Refresh();

		aim::Refresh();

		agl::UpdateFrame();
	}

	entity->Destroy();

	agl_ext::UninstallAll();
	agl::Destroy();

	return 0;

}
