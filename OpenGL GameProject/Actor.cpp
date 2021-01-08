#include"Actor.h"
#include"Component.h"
#include"Game.h"
#include<algorithm>

Actor::Actor(Game* game)
	:mState(State::EActive)
	,m2Position(Vector2::Zero)
	,mScale(1.0f)
	,m2Rotation(0.0f)
	,mGame(game)
	,mRecomputeWorldTransform(false)
{
	mGame->AddActor(this);
}

Actor::~Actor()
{
	mGame->RemoveActor(this);
	// Need to delete components
	// Already have RemoveComponent in ~Component, need a different style loop
	while (!mComponents.empty())
	{
		delete mComponents.back();
	}
}

void Actor::Update(float deltaTime)
{
	if (mState == State::EActive)
	{
		ComputeWorldTransform();
		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);
		ComputeWorldTransform();
	}
}

void Actor::UpdateActor(float deltaTime)
{

}

void Actor::UpdateComponents(float deltaTime)
{
	//First process input for components
	for (auto comp : mComponents)
	{
		comp->Update(deltaTime);
	}
}

void Actor::ProcessInput(const uint8_t* keyState)
{
	if (mState == State::EActive)
	{
		// First process input for components
		for (auto comp : mComponents)
		{
			comp->ProcessInput(keyState);
		}

		ActorInput(keyState);
	}
}

void Actor::ActorInput(const uint8_t* keyState)
{
}

void Actor::AddComponent(Component* component)
{
	//Find the insertion point in the sorted vector
	int myOrder = component->GetUpdateOrder();
	auto iter = mComponents.begin();
	for (; iter != mComponents.end(); iter++)
	{
		if (myOrder < (*iter)->GetUpdateOrder())
		{
			break;
		}
	}
	//Inserts element before position of iterator
	mComponents.insert(iter, component);
}

void Actor::RemoveComponent(Component* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	if (iter != mComponents.end())
	{
		mComponents.erase(iter);
	}
}

void Actor::ComputeWorldTransform()
{
	if (mRecomputeWorldTransform)
	{
		mRecomputeWorldTransform = false;
		//Scale, then rotate, then translate
		mWorldTransform = Matrix4::CreateScale(mScale);
		mWorldTransform *= Matrix4::CreateRotationZ(m2Rotation);
		mWorldTransform *= Matrix4::CreateTranslation(Vector3(m2Position.x, m2Position.y, 0.0f));
		mWorldTransform *= Matrix4::CreateFromQuaternion(m3Rotation);
		mWorldTransform *= Matrix4::CreateTranslation(m3Position);
		//Inform components world transform updated
		for (auto comp : mComponents)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}