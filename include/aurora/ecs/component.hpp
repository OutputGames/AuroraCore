#if !defined(COMPONENT_HPP)
#define COMPONENT_HPP

#define TO_STRING(x) #x

//****************
#define CLASS_DECLARATION( classname )                                                      \
public:                                                                                     \
    static const std::size_t Type;                                                          \
    virtual bool IsClassType( const std::size_t classType ) const override;                 \
    virtual Ref<Component> clone() const override \
	{\
		return Ref<Component>(std::make_shared<classname>(*this));\
    }\

//****************
// CLASS_DEFINITION
// 
// This macro must be included in the class definition to properly initialize 
// variables used in type checking. Take special care to ensure that the 
// proper parentclass is indicated or the run-time type information will be
// incorrect. Only works on single-inheritance RTTI.
//****************
#define CLASS_DEFINITION( parentclass, childclass )                                         \
const std::size_t childclass::Type = std::hash< const char* >()( TO_STRING( childclass ) ); \
bool childclass::IsClassType( const std::size_t classType ) const {                         \
        if ( classType == childclass::Type )                                                \
            return true;                                                                    \
        return parentclass::IsClassType( classType );                                       \
}                                                                                           \


#define QUICKCLASS(parentclass, classname) \
class AURORA_API classname : public parentclass\
{\
    CLASS_DECLARATION(classname)\
public:\
    classname(std::string&& initialValue)\
        : parentclass(move(initialValue)) {\
    }\
    classname() = default; \

#include <memory>

#include "aurora/math/math.hpp"
#include "aurora/utils/utils.hpp"

class Collider;
struct Collision;
struct Entity;

class AURORA_API Component {
public:
    static const std::size_t                    Type;
    virtual bool                                IsClassType(const std::size_t classType) const {
        return classType == Type;
    }
    virtual Ref<Component> clone() const
    {
        return Ref(std::make_shared<Component>(*this));
    }
public:

    virtual                                ~Component() = default;
    Component(std::string&& initialValue)
        : value(initialValue) {
    }

    Component() = default;

    Entity* Entity;
    bool Enabled = true;

    virtual void Init() {}
    virtual void Update() {}
    virtual void Destroy() {}

    virtual void OnCollisionEnter(Collision* Collision) {}
    virtual void OnCollisionStay(Collision* Collision) {}
    virtual void OnCollisionExit(Collider* Collision) {}

	virtual nlohmann::json Serialize();
    virtual void Load(nlohmann::json j);

    std::string ToString();

private:
    std::string                             value = "uninitialized";

    friend struct Entity;

    bool Initialized;

};

class TestComponent : public Component
{

    CLASS_DECLARATION(TestComponent)

public:
	TestComponent(std::string&& initialValue) : Component(move(initialValue))
	{
	}

	TestComponent() = default;

    void Init() override;
    void Update() override;

};

#endif // COMPONENT_HPP
