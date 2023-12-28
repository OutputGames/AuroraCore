#include "aurora.hpp"

#include "ecs/audio.hpp"
#include "ecs/registry.hpp"
#include "gfx/camera.hpp"
#include "gfx/lighting.hpp"
#include "physics/physics.hpp"
#include "gfx/renderer.hpp"

void aclComponentManager::Init()
{

	ComponentRegistry::RegisterComponent(ComponentRegistry::ComponentRegister<AudioPlayer, Light, MeshRenderer, Camera, Rigidbody, BoxCollider, SphereCollider, CapsuleCollider, CharacterController, MeshCollider>());
}
