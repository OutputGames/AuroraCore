#include "physics.hpp"

#include "aurora/ecs/entity.hpp"
#include "aurora/ecs/scene.hpp"
#include "Jolt/Physics/Collision/Shape/ScaledShape.h"

CLASS_DEFINITION(Component, Collider)
CLASS_DEFINITION(Collider, BoxCollider)
CLASS_DEFINITION(Collider, SphereCollider)
CLASS_DEFINITION(Collider, CapsuleCollider)
CLASS_DEFINITION(Component, CharacterController)
CLASS_DEFINITION(Component, Rigidbody)
CLASS_DEFINITION(Collider, MeshCollider)

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

nlohmann::json SphereCollider::Serialize()
{
	nlohmann::json j = Collider::Serialize();

	j["Radius"] = Radius;

	return j;
}

void SphereCollider::Load(nlohmann::json j)
{
	Radius = j["Radius"];
}

void Rigidbody::Init()
{
	collider = Entity->GetComponent<Collider>();

	JPH::ObjectLayer layer = GetObjectLayer();

	JPH::ShapeSettings* settings = collider->GetShapeSettings();


	JPH::Shape* shape = settings->Create().Get();
	if (shape->IsValidScale(aclPhysicsMgr::GetVec3(Entity->Transform.scale))) {
		//shape. = shape->ScaleShape(aclPhysicsMgr::GetVec3(Entity->Transform.scale)).Get();
	}

	collider->Center *= Entity->Transform.scale;

	body = aclPhysicsMgr::CreateBody(aclPhysicsMgr::GetCreationSettings(shape, Entity->Transform.position, quat(Entity->Transform.rotation * vec3{DEG2RAD}), static_cast<JPH::EMotionType>(motionType), layer));

	body->SetUserData(Entity->Id);

	initialPosition = Entity->Transform.position;
	initialRotation = Entity->Transform.rotation;

}

void Rigidbody::Update()
{

	vec3 pos = aclPhysicsMgr::GetVec3(body->GetCenterOfMassPosition());
	vec3 rot = aclPhysicsMgr::GetVec3(body->GetRotation().GetEulerAngles());

	/*
	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_POSITION_X))	
		aclPhysicsMgr::bodyInterface->SetPosition(body->GetID(), aclPhysicsMgr::GetVec3(vec3{ initialPosition.x,pos.y,pos.z }), GetActivationType());
	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_POSITION_Y))	
		aclPhysicsMgr::bodyInterface->SetPosition(body->GetID(), aclPhysicsMgr::GetVec3(vec3{ pos.x,initialPosition.y,pos.z }), GetActivationType());
	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_POSITION_Z))
		aclPhysicsMgr::bodyInterface->SetPosition(body->GetID(), aclPhysicsMgr::GetVec3(vec3{ pos.x,pos.y,initialPosition.z}), GetActivationType());

	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_ROTATION_X))	
		aclPhysicsMgr::bodyInterface->SetRotation(body->GetID(), aclPhysicsMgr::GetQuat(vec3{ initialRotation.x*DEG2RAD,rot.y,rot.z }), GetActivationType());
	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_ROTATION_Y))
		aclPhysicsMgr::bodyInterface->SetRotation(body->GetID(), aclPhysicsMgr::GetQuat(vec3{ rot.x,initialRotation.y*DEG2RAD,rot.z }), GetActivationType());
	if (uvector::ContainsElement<ConstraintType>(constraints, LOCK_ROTATION_Z))
		aclPhysicsMgr::bodyInterface->SetRotation(body->GetID(), aclPhysicsMgr::GetQuat(vec3{ rot.x,rot.y,initialRotation.z*DEG2RAD }), GetActivationType());
		*/

	Entity->Transform.position = aclPhysicsMgr::GetVec3(body->GetCenterOfMassPosition()) + (collider->Center * Entity->Transform.scale);

	Entity->Transform.rotation = aclPhysicsMgr::GetVec3(body->GetRotation().GetEulerAngles()) * vec3 { RAD2DEG };

}

void Rigidbody::SetPosition(vec3 position)
{
	aclPhysicsMgr::bodyInterface->SetPosition(body->GetID(), aclPhysicsMgr::GetVec3(position), GetActivationType());
}

void Rigidbody::SetRotation(vec3 rotation)
{
	aclPhysicsMgr::bodyInterface->SetPositionAndRotation(body->GetID(), body->GetCenterOfMassPosition(), aclPhysicsMgr::GetQuat(rotation), GetActivationType());
		
}

void Rigidbody::SetVelocity(vec3 velocity)
{
	aclPhysicsMgr::bodyInterface->SetLinearVelocity(body->GetID(), aclPhysicsMgr::GetVec3(velocity));
}

void Rigidbody::ApplyForce(vec3 force)
{
	aclPhysicsMgr::bodyInterface->MoveKinematic(body->GetID(), aclPhysicsMgr::GetVec3(Entity->Transform.position+force), body->GetRotation(), 1.0f/60.0f);
}

void Rigidbody::SetRestitution(float f)
{
	body->SetRestitution(f);
}

void Rigidbody::ApplyConstraint(ConstraintType type)
{
	constraints.push_back(type);
}

nlohmann::json Rigidbody::Serialize()
{
	nlohmann::json j = Component::Serialize();

	j["MotionType"] = motionType;

	return j;
}

void Rigidbody::Load(nlohmann::json j)
{
	motionType = j["MotionType"];
}

void Rigidbody::Reset()
{
	SetPosition(initialPosition);
	SetRotation(initialRotation);
	SetVelocity(vec3{ 0 });
}

void CharacterController::Init()
{
	collider = Entity->GetComponent<Collider>();

	JPH::ObjectLayer layer = GetObjectLayer();

	// Create 'player' character
	JPH::CharacterSettings* settings = new JPH::CharacterSettings();
	settings->mMaxSlopeAngle = JPH::DegreesToRadians(45.0f);
	settings->mLayer = layer;
	settings->mShape = collider->GetShapeSettings()->Create().Get();

	if (settings->mShape->IsValidScale(aclPhysicsMgr::GetVec3(Entity->Transform.scale))) {
		settings->mShape = settings->mShape->ScaleShape(aclPhysicsMgr::GetVec3(Entity->Transform.scale)).Get();
	}

	float radius = settings->mShape->GetInnerRadius();

	settings->mFriction = 0.5f;
	settings->mSupportingVolume = JPH::Plane(JPH::Vec3::sAxisY(), radius); // Accept contacts that touch the lower sphere of the capsule

	body = aclPhysicsMgr::CreateCharacter(settings, 
		Entity->Transform.position,
          quat(Entity->Transform.rotation * vec3{DEG2RAD}), GetActivationType(),
          Entity->Id);

	initialPosition = Entity->Transform.position;
	initialRotation = Entity->Transform.rotation;
}

void CharacterController::Update()
{
	Entity->Transform.position = aclPhysicsMgr::GetVec3(body->GetCenterOfMassPosition()) + (collider->Center * Entity->Transform.scale);

	Entity->Transform.rotation = aclPhysicsMgr::GetVec3(body->GetRotation().GetEulerAngles()) * vec3 { RAD2DEG };
}

void CharacterController::SetPosition(vec3 position)
{
	aclPhysicsMgr::bodyInterface->SetPosition(body->GetBodyID(), aclPhysicsMgr::GetVec3(position), GetActivationType());
}

void CharacterController::SetRotation(vec3 rotation)
{
	aclPhysicsMgr::bodyInterface->SetRotation(body->GetBodyID(),aclPhysicsMgr::GetQuat(rotation), GetActivationType());
}

void CharacterController::SetVelocity(vec3 velocity)
{
	aclPhysicsMgr::bodyInterface->SetLinearVelocity(body->GetBodyID(), aclPhysicsMgr::GetVec3(velocity));
}

vec3 CharacterController::GetVelocity()
{
	return aclPhysicsMgr::GetVec3(aclPhysicsMgr::bodyInterface->GetLinearVelocity(body->GetBodyID()));
}

JPH::Character::EGroundState CharacterController::GetState()
{
	return body->GetGroundState();
}

void CharacterController::Reset()
{
	SetPosition(initialPosition);
	//SetRotation(initialRotation);
	SetVelocity(vec3{ 0 });
}

JPH::ShapeSettings* MeshCollider::GetShapeSettings()
{

	JPH::VertexList list;
	JPH::IndexedTriangleList triList;

	for (auto vertex : mesh->vertices)
	{
		list.push_back(aclPhysicsMgr::GetFloat3(vertex.position * Entity->Transform.scale));
	}

	for (int i = 0; i < mesh->indices.size(); i += 3)
	{
		JPH::IndexedTriangle tri(mesh->indices[i], mesh->indices[i + 1], mesh->indices[i + 2]);
		triList.push_back(tri);
	}



	return new JPH::MeshShapeSettings{ list,triList };
}

void MeshCollider::ApplyShape(JPH::Shape* shape)
{
	this->shape = static_cast<JPH::MeshShape*>(shape);
}


JPH::ShapeSettings* BoxCollider::GetShapeSettings()
{
	return new JPH::BoxShapeSettings(aclPhysicsMgr::GetVec3(Extents * Entity->Transform.scale / vec3{ 2.0f }));
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

nlohmann::json BoxCollider::Serialize()
{
	nlohmann::json j = Collider::Serialize();

	j["Extents"] = MathSerializer::SerializeVec3(Extents);

	return j;
}

void BoxCollider::Load(nlohmann::json j)
{
	Extents = MathSerializer::DeserializeVec3(j["Extents"]);
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

JPH::ValidateResult aclContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2,
	JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
{
	//std::cout << "Contact validate callback" << std::endl;

	// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
	return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
}

void aclContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2,
	const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{

	Ref<Entity> entity1 = Scene::Current->entityMgr->registeredEntities[inBody1.GetUserData()];
	Ref<Entity> entity2 = Scene::Current->entityMgr->registeredEntities[inBody2.GetUserData()];

	Collision* collision = new Collision;

	collision->collider1 = entity1->GetComponent<Collider>();
	collision->collider2 = entity2->GetComponent<Collider>();

	collision->hitNormal = aclPhysicsMgr::GetVec3(inManifold.mWorldSpaceNormal);

	int i = 0;

	for (auto m_relative_contact_points_on1 : inManifold.mRelativeContactPointsOn1)
	{
		collision->collider1ContactPoints.push_back(aclPhysicsMgr::GetVec3(inManifold.GetWorldSpaceContactPointOn1(i)));
		i++;
	}

	i = 0;
	for (auto m_relative_contact_points_on1 : inManifold.mRelativeContactPointsOn2)
	{
		collision->collider2ContactPoints.push_back(aclPhysicsMgr::GetVec3(inManifold.GetWorldSpaceContactPointOn2(i)));
		i++;
	}

	entity1->OnCollisionEnter(collision);
	entity2->OnCollisionEnter(collision);

	delete collision;

	//std::cout << "A contact was added" << std::endl;
}

void aclContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2,
	const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
{


	Ref<Entity> entity1 = Scene::Current->entityMgr->registeredEntities[inBody1.GetUserData()];
	Ref<Entity> entity2 = Scene::Current->entityMgr->registeredEntities[inBody2.GetUserData()];

	Collision* collision = new Collision;

	collision->collider1 = entity1->GetComponent<Collider>();
	collision->collider2 = entity2->GetComponent<Collider>();

	collision->hitNormal = aclPhysicsMgr::GetVec3(inManifold.mWorldSpaceNormal);

	int i = 0;

	for (auto m_relative_contact_points_on1 : inManifold.mRelativeContactPointsOn1)
	{
		collision->collider1ContactPoints.push_back(aclPhysicsMgr::GetVec3(inManifold.GetWorldSpaceContactPointOn1(i)));
		i++;
	}

	i = 0;
	for (auto m_relative_contact_points_on1 : inManifold.mRelativeContactPointsOn2)
	{
		collision->collider2ContactPoints.push_back(aclPhysicsMgr::GetVec3(inManifold.GetWorldSpaceContactPointOn2(i)));
		i++;
	}

	entity1->OnCollisionStay(collision);
	entity2->OnCollisionStay(collision);

	delete collision;

	//std::cout << "A contact was persisted" << std::endl;
}

void aclContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
{

	//std::cout << "A contact was removed" << std::endl;
}

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

	if (frameCount == 0)
	{
		system->OptimizeBroadPhase();
	}

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

	frameCount++;
}

void aclPhysicsMgr::Destroy()
{
	if (bodies.size() > 0) {
		bodyInterface->RemoveBodies(bodies.data(), bodies.size());
		bodyInterface->DestroyBodies(bodies.data(), bodies.size());
	}

	bodies.clear();
}

void aclPhysicsMgr::Uninitialize()
{
	// Unregisters all types with the factory and cleans up the default material
	JPH::UnregisterTypes();

	// Destroy the factory
	delete JPH::Factory::sInstance;
	JPH::Factory::sInstance = nullptr;
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

	bodies.push_back(body->GetID());

	return body;
}

JPH::Character* aclPhysicsMgr::CreateCharacter(JPH::CharacterSettings* settings, vec3 position, quat rotation, JPH::EActivation activation, u64 userData)
{
	JPH::Character* mCharacter = new JPH::Character(settings, GetVec3(position), GetQuat(rotation), userData, system);
	mCharacter->AddToPhysicsSystem(activation);

	return mCharacter;
}

JPH::BodyCreationSettings aclPhysicsMgr::GetCreationSettings(JPH::Shape* shape, vec3 position, quat rotation,
                                                             JPH::EMotionType motion, JPH::ObjectLayer layer)
{
	return { shape, JPH::RVec3(position.x, position.y, position.z), JPH::Quat(rotation.x, rotation.y, rotation.z, rotation.w), motion, layer };
}

JPH::ShapeSettings* Collider::GetShapeSettings()
{
	return nullptr;
}

void Collider::ApplyShape(JPH::Shape* shape)
{

}

nlohmann::json Collider::Serialize()
{
	json j;

	j["Center"] = MathSerializer::SerializeVec3(Center);

	return j;
}

void Collider::Load(nlohmann::json j)
{
	Center = MathSerializer::DeserializeVec3(j["Center"]);
}

void CapsuleCollider::ApplyShape(JPH::Shape* shape)
{
	this->shape = static_cast<JPH::CapsuleShape*>(shape);
}

JPH::Shape* CapsuleCollider::GetShape()
{
	return shape;
}

JPH::ShapeSettings* CapsuleCollider::GetShapeSettings()
{
	return new JPH::CapsuleShapeSettings{ Height, Radius };
}

void CapsuleCollider::Load(nlohmann::json j)
{
	Radius = j["Radius"];
	Height = j["Height"];
}

nlohmann::json CapsuleCollider::Serialize()
{
	json j = Collider::Serialize();

	j["Radius"] = Radius;
	j["Height"] = Height;

	return j;
}


void CapsuleCollider::Update()
{
	
}
