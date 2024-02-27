#pragma once

class Entity;

class Components
{
public:
	Components();
	~Components() {};

	// Init
	virtual void Init() {};

	// SETTER / GETTER 


private:
	Entity* m_pEntity; 
};

