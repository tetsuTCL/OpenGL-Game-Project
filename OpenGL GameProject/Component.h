#pragma once
#include"Actor.h"
#include<cstdint>

class Component
{
public:
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();
	virtual void Update(float deltaTime);
	virtual void ProcessInput(const uint8_t* keyState) {}
	// Called when world transform changes
	virtual void OnUpdateWorldTransform(){ }

	int GetUpdateOrder()const { return mUpdateOrder; }

protected:
	class Actor* mOwner;
	int mUpdateOrder;

};

