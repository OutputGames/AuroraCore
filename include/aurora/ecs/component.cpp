#include "component.hpp"

#include "entity.hpp"

using namespace std;

const std::size_t Component::Type = std::hash<const char*>()(TO_STRING(Component));

CLASS_DEFINITION(Component, TestComponent)

void TestComponent::Init()
{
	cout << "TestComponent created" << endl;
}

void TestComponent::Update()
{
	//cout << "TestComponent updated" << endl;
}

nlohmann::json Component::Serialize()
{
	nlohmann::json j;

	j["Entity"] = Entity->Id;
	j["Enabled"] = Enabled;
	j["Name"] = ToString();

	return j;
}

void Component::Load(nlohmann::json j)
{
}

string Component::ToString()
{
	std::string rawname = typeid(*this).name();

	std::string sub = "class ";

	std::string::size_type i = rawname.find(sub);

	if (i != std::string::npos)
		rawname.erase(i, sub.length());

	return rawname;
}