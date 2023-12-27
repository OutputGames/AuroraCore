#if !defined(PHYSICS_HPP)
#define PHYSICS_HPP

#include "aurora/utils/utils.hpp"

#include "Jolt/Jolt.h"
// Jolt includes
#include <Jolt/RegisterTypes.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>

#include "aurora/ecs/component.hpp"
#include "Jolt/Physics/Character/Character.h"
#include "Jolt/Physics/Collision/Shape/CapsuleShape.h"

// Layer that objects can be in, determines which other objects it can collide with
// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
// but only if you do collision testing).
namespace Layers
{
	static constexpr JPH::ObjectLayer NON_MOVING = 0;
	static constexpr JPH::ObjectLayer MOVING = 1;
	static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
};

/// Class that determines if two object layers can collide
class AURORA_API aclObjectLayerPairFilter : public JPH::ObjectLayerPairFilter
{
public:
	virtual bool					ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
namespace BroadPhaseLayers
{
	static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
	static constexpr JPH::BroadPhaseLayer MOVING(1);
	static constexpr u32 NUM_LAYERS(2);
};

// BroadPhaseLayerInterface implementation
// This defines a mapping between object and broadphase layers.
class AURORA_API aclBPLayerInterface final : public JPH::BroadPhaseLayerInterface
{
public:
	aclBPLayerInterface()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	virtual uint					GetNumBroadPhaseLayers() const override
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	virtual JPH::BroadPhaseLayer			GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
	{
		switch ((BroadPhaseLayer::Type)inLayer)
		{
		case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

private:
	JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
};

/// Class that determines if an object layer can collide with a broadphase layer
class AURORA_API aclObjectBroadPhaseLayerFilter : public JPH::ObjectVsBroadPhaseLayerFilter
{
public:
	virtual bool				ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
};

// An example contact listener
class AURORA_API aclContactListener : public JPH::ContactListener
{
public:
	// See: ContactListener
	virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;

	virtual void			OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

	virtual void			OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

	virtual void			OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;
};

// An example activation listener
class AURORA_API aclBodyActivationListener : public JPH::BodyActivationListener
{
public:
	virtual void		OnBodyActivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData) override
	{
		//std::cout << "A body got activated" << std::endl;
	}

	virtual void		OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64 inBodyUserData) override
	{
		//std::cout << "A body went to sleep" << std::endl;
	}
};

struct AURORA_API aclPhysicsMgr
{
	IS JPH::PhysicsSystem* system;
	IS aclBodyActivationListener bodyActivationListener = {};
	IS aclContactListener contactListener = {};
	IS JPH::TempAllocatorImpl* tempAllocator;
	IS JPH::JobSystemThreadPool* jobSystem;
	IS aclBPLayerInterface broadphaseInterface = {};
	IS aclObjectBroadPhaseLayerFilter objectBroadPhaseLayer = {};
	IS aclObjectLayerPairFilter layerPair = {};
	IS JPH::BodyInterface* bodyInterface;

	static void Setup();
	static void Update(float deltaTime);
	static void Destroy();
	static void Uninitialize();

	static JPH::Body* CreateBody(JPH::BodyCreationSettings settings);

	static JPH::BodyCreationSettings GetCreationSettings(JPH::ShapeSettings* settings, vec3 position, quat rotation, JPH::EMotionType motion, JPH::ObjectLayer layer);

	static vec3 GetVec3(JPH::RVec3 v)
	{
		return { v.GetX(), v.GetY(), v.GetZ() };
	}

	static quat GetQuat(JPH::Quat q)
	{
		return { q.GetX(), q.GetY(), q.GetZ(),q.GetW() };
	}

	static JPH::Quat GetQuat(vec3 v)
	{
		return { v.x, v.y, v.z,1.0 };
	}

	static JPH::RVec3 GetVec3(vec3 v)
	{
		return JPH::RVec3(v.x, v.y, v.z);
	}

private:

	IS std::vector<JPH::BodyID> bodies;


	IS int frameCount;
};

class AURORA_API Collider : public Component
{
	CLASS_DECLARATION(Collider)

public:
	Collider(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	Collider() = default;

	virtual JPH::ShapeSettings* GetShapeSettings();
	virtual void ApplyShape(JPH::Shape* shape);
	virtual JPH::Shape* GetShape() {
		return nullptr;
	}

private:



};

class AURORA_API BoxCollider : public Collider
{
	CLASS_DECLARATION(BoxCollider)

public:
	BoxCollider(std::string&& initialValue) : Collider(move(initialValue))
	{
	}

	BoxCollider() = default;

	vec3 Extents = vec3{1};

	JPH::ShapeSettings* GetShapeSettings() override;
	void Update() override;
	void ApplyShape(JPH::Shape* shape) override;
	JPH::Shape* GetShape() override {
		return nullptr;
	}

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

private:

	JPH::BoxShape* shape;

};

struct Collision
{
	Collider* collider1;
	Collider* collider2;

	vec3 hitNormal;

	std::vector<vec3> collider1ContactPoints;
	std::vector<vec3> collider2ContactPoints;

};

class AURORA_API SphereCollider : public Collider
{
	CLASS_DECLARATION(SphereCollider)

	

public:
	SphereCollider(std::string&& initialValue) : Collider(move(initialValue))
	{
	}


	SphereCollider() = default;

	float Radius=1;

	JPH::ShapeSettings* GetShapeSettings() override;
	void Update() override;
	void ApplyShape(JPH::Shape* shape) override;
	JPH::Shape* GetShape() override {
		return shape;
	}

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

private:

	JPH::SphereShape* shape;
};

class AURORA_API CapsuleCollider : public Collider
{
	CLASS_DECLARATION(CapsuleCollider)

public:
	CapsuleCollider(std::string&& initialValue) : Collider(move(initialValue))
	{
	}

	CapsuleCollider() = default;

	float Radius = 1;
	float Height = 2;

	JPH::ShapeSettings* GetShapeSettings() override;
	void Update() override;
	void ApplyShape(JPH::Shape* shape) override;
	JPH::Shape* GetShape() override;

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

private:

	JPH::CapsuleShape* shape;

};

class AURORA_API Rigidbody : public Component
{
	CLASS_DECLARATION(Rigidbody)

public:
	Rigidbody(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	Rigidbody() = default;

	void Init() override;
	void Update() override;
	void SetPosition(vec3 position);
	void SetRotation(vec3 rotation);
	void SetVelocity(vec3 velocity);

	void ApplyForce(vec3 force);

	void SetRestitution(float f);


	JPH::EActivation GetActivationType()
	{
		switch (motionType)
		{
		case STATIC:
			return JPH::EActivation::DontActivate;
			break;
		case DYNAMIC:
			return JPH::EActivation::Activate;
			break;
		default:
			return JPH::EActivation::Activate;
			break;
		}
	}

	JPH::ObjectLayer GetObjectLayer()
	{
		switch (motionType)
		{
		case STATIC:
			return Layers::NON_MOVING;
			break;
		case DYNAMIC:
			return Layers::MOVING;
			break;
		default:
			return Layers::MOVING;
			break;
		}
	}

	enum MotionType
	{
		STATIC,
		KINEMATIC,
		DYNAMIC
	} motionType = DYNAMIC;

	enum ConstraintType
	{
		LOCK_POSITION_X = 1 << 0,
		LOCK_POSITION_Y = 1 << 1,
		LOCK_POSITION_Z = 1 << 2,
		LOCK_ROTATION_X = 1 << 3,
		LOCK_ROTATION_Y = 1 << 4,
		LOCK_ROTATION_Z = 1 << 5,
	} constraint;

	nlohmann::json Serialize() override;
	void Load(nlohmann::json j) override;

	JPH::Body* operator->()
	{
		return body;
	}

	void Reset();

private:

	JPH::Body* body=nullptr;

	glm::vec3 initialPosition, initialRotation, initialScale;

};


#endif // PHYSICS_HPP
