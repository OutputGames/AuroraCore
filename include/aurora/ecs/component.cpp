#include "component.hpp"

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
