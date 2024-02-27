#pragma once
#include <vector>

class Component;

class Entity
{
public:
	Entity();
	~Entity() {};

	// INIT
	void Init();

	// SETTER / GETTER
	template<typename T>
	void AddComponent()
	{
		for (int i = 0; i < m_componentsList.size(); i++)
		{
			if (m_componentsList[i] == T)
				return;
		}
		m_componentsList.push_back(T);
	}

	template<typename T>
	void RemoveComponent()
	{
		for (int i = 0; i < m_componentsList.size(); i++)
		{
			if (m_componentsList[i] == T)
			{
				m_componentsList.erase(m_componentsList.begin() + i);
			}

			return;
		}
	}
	
	template<typename T>
	T GetComponent() {
		for (int i = 0; i < m_componentsList.size(); i++)
		{
			// TO DO: Code pour trouver le bon component dans m_componentsList
			// si trouvé -> le retourner 
			// sinon en bas
			if (m_componentsList[i] == T)
			{
				return m_componentsList[i];
			}
		}
		return nullptr;
	}

	// CLEAR
	void Clear();

private:
	std::vector<Component*> m_componentsList; 
};

