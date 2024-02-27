#include "Entity.h"

Entity::Entity()
{
}

void Entity::AddComponent(Component* component)
{
	for (int i = 0; i < m_componentsList.size(); i++)
	{
		if (m_componentsList[i] == component)
			return;
	}
	m_componentsList.push_back(component);
}

void Entity::Clear()
{
	for (int i = 0; i < m_componentsList.size(); i++)
	{
		m_componentsList.erase(m_componentsList.begin() + i);
	}
}

