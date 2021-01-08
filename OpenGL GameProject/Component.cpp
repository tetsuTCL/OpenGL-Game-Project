#include"Component.h"
#include"Actor.h"

Component::Component(Actor* owner, int drawOrder)
	:mOwner(owner)
	,mUpdateOrder(drawOrder)
{
	mOwner->AddComponent(this);
}

Component::~Component()
{
	mOwner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{

}