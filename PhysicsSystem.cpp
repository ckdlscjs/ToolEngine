#include "PhysicsSystem.h"
#include "CameraSystem.h"

FSelect& PhysicsSystem::GetSelect()
{
	return m_Select;
}
void PhysicsSystem::Update()
{
	m_Select.SetMatrix(nullptr, &_CameraSystem.GetCurrentCamera()->m_matCamera, &_CameraSystem.GetCurrentCamera()->m_matProj);
}

PhysicsSystem::PhysicsSystem()
{
	std::cout << "Initialize : PhysicsSystem" << std::endl;
}

PhysicsSystem::~PhysicsSystem()
{
	std::cout << "Release : PhysicsSystem" << std::endl;
}
	
