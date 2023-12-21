#include "physics.hpp"

#include "aurora/ecs/entity.hpp"

CLASS_DEFINITION(Component, Collider)
CLASS_DEFINITION(Collider, BoxCollider)
CLASS_DEFINITION(Collider, SphereCollider)
CLASS_DEFINITION(Component, Rigidbody)

JPH::ShapeSettings* SphereCollider::GetShapeSettings()
{
	return new JPH::SphereShapeSettings(Radius);
}

void SphereCollider::Update()
{
	static float rad = Radius;

	if (Radius != rad)
	{
		rad = Radius;

		delete shape;

		JPH::SphereShape* shape = new JPH::SphereShape(rad);
		ApplyShape(shape);
	}
}

void SphereCollider::ApplyShape(JPH::Shape* shape)
{
	this->shape = static_cast<JPH::SphereShape*>(shape);
}

void Rigidbody::Init()
{
	Collider* collider = Entity->GetComponent<Collider>();

	JPH::ObjectLayer layer = GetObjectLayer();

	body = aclPhysicsMgr::CreateBody(aclPhysicsMgr::GetCreationSettings(collider->GetShapeSettings(), Entity->Transform.position, quat(Entity->Transform.rotation), static_cast<JPH::EMotionType>(motionType), layer));

	initialPosition = Entity->Transform.position;

}

void Rigidbody::Update()
{
	Entity->Transform.position = aclPhysicsMgr::GetVec3(body->GetCenterOfMassPosition());

	Entity->Transform.rotation = aclPhysicsMgr::GetVec3(body->GetRotation().GetEulerAngles()) * vec3 { RAD2DEG };

}

void Rigidbody::SetPosition(vec3 position)
{
	aclPhysicsMgr::bodyInterface->SetPosition(body->GetID(), aclPhysicsMgr::GetVec3(position), GetActivationType());
}

void Rigidbody::SetVelocity(vec3 velocity)
{
	aclPhysicsMgr::bodyInterface->SetLinearVelocity(body->GetID(), aclPhysicsMgr::GetVec3(velocity));
}

void Rigidbody::SetRestitution(float f)
{
	body->SetRestitution(f);
}

void Rigidbody::Reset()
{
	SetPosition(initialPosition);
	SetVelocity(vec3{ 0 });
}

JPH::ShapeSettings* BoxCollider::GetShapeSettings()
{
	return new JPH::BoxShapeSettings(aclPhysicsMgr::GetVec3(Extents / vec3{ 2 }));
}

void BoxCollider::Update()
{
	static vec3 ext = Extents;

	if (Extents != ext)
	{
		ext = Extents;

		delete shape;

		JPH::BoxShape* shape = new JPH::BoxShape(aclPhysicsMgr::GetVec3(ext));
		ApplyShape(shape);
	}
	
}

void BoxCollider::ApplyShape(JPH::Shape* shape)
{
	this->shape = static_cast<JPH::BoxShape*>(shape);
}


// Callback for traces, connect this to your own trace function if you have one
static void TraceImpl(const char* inFMT, ...)
{
	// Format the message
	va_list list;
	va_start(list, inFMT);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), inFMT, list);
	va_end(list);

	// Print to the TTY
	std::cout << buffer << std::endl;
}

#ifdef JPH_ENABLE_ASSERTS

// Callback for asserts, connect this to your own assert handler if you have one
static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, uint inLine)
{
	// Print to the TTY
	std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

	// Breakpoint
	return true;
};

#endif // JPH_ENABLE_ASSERTS

void aclPhysicsMgr::Setup()
{
	// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodies = 1024;

	// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
	const uint cNumBodyMutexes = 0;

	// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
	// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
	// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
	const uint cMaxBodyPairs = 1024;

	// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
	// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
	// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
	const uint cMaxContactConstraints = 1024;


	JPH::RegisterDefaultAllocator();

	JPH::Trace = TraceImpl;
	JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl;)

	JPH::Factory::sInstance = new JPH::Factory();

	JPH::RegisterTypes();

	// Create a factory

	tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);

	jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);
	system = new JPH::PhysicsSystem;
	

	system->Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, broadphaseInterface, objectBroadPhaseLayer, layerPair);

	system->SetBodyActivationListener(&bodyActivationListener);
	system->SetContactListener(&contactListener);

	bodyInterface = &system->GetBodyInterface();


}

void aclPhysicsMgr::Update(float cDeltaTime)
{
	// We simulate the physics world in discrete time steps. 60 Hz is a good rate to update the physics system.

	// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
	int cCollisionSteps = (1.0f / 60.0f) / cDeltaTime;

	if (cCollisionSteps == 0)
	{
		cCollisionSteps += 1;
		cDeltaTime = 1.0f / 60.0f;
	}

	// Step the world
	system->Update(cDeltaTime, cCollisionSteps, tempAllocator, jobSystem);
}

JPH::Body* aclPhysicsMgr::CreateBody(JPH::BodyCreationSettings settings)
{

	auto body = bodyInterface->CreateBody(settings);

	if (body == nullptr)
	{
		throw std::exception("We have hit the maximum amount of bodies.");
	}

	JPH::EActivation activation;

	if (body->GetMotionType() == JPH::EMotionType::Static)
	{
		activation = JPH::EActivation::DontActivate;
	} else
	{
		activation = JPH::EActivation::Activate;
	}

	bodyInterface->AddBody(body->GetID(), activation);

	return body;
}

JPH::BodyCreationSettings aclPhysicsMgr::GetCreationSettings(JPH::ShapeSettings* settings, vec3 position, quat rotation,
	JPH::EMotionType motion, JPH::ObjectLayer layer)
{
	JPH::ShapeRefC shape = settings->Create().Get();
	return { shape, JPH::RVec3(position.x, position.y, position.z), JPH::Quat(rotation.x, rotation.y, rotation.z, rotation.w), motion, layer };
}

JPH::ShapeSettings* Collider::GetShapeSettings()
{
	return nullptr;
}

void Collider::ApplyShape(JPH::Shape* shape)
{

}
