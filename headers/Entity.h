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
	void AddComponent(Component* component);
	void RemoveComponent(Component* component);

	template<typename T>
	T GetComponent() {
		for (int i = 0; i < m_componentsList.size(); i++)
		{
			// TO DO: Code pour trouver le bon component dans m_componentsList
			// si trouvé -> le retourner 
			// sinon en bas
		}
		return nullptr;
	}

	// CLEAR
	void Clear();

private:
	std::vector<Component*> m_componentsList; 
};

