#include "Entity.h"

Entity::Entity()
{
}

void Entity::Init()
{
	// AddComponent<Transform>();
}

void Entity::Clear()
{
	for (int i = 0; i < m_componentsList.size(); i++)
	{
		m_componentsList.erase(m_componentsList.begin() + i);
	}
}

