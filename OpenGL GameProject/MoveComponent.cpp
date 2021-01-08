#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		Quaternion rotation = mOwner->GetQRotation();
		float angle = mAngularSpeed * deltaTime;
		// Create quaternion for incremental rotation
		// (Rotate about up axis)
		Quaternion increment(Vector3::UnitZ, angle);
		// Concatenate old and new quaternion
		rotation = Quaternion::Concatenate(rotation, increment);
		mOwner->SetQRotation(rotation);
	}

	if (!Math::NearZero(mForwardSpeed))
	{
		Vector3 position = mOwner->GetVec3Position();
		position += mOwner->GetForward() * mForwardSpeed * deltaTime;
		mOwner->SetVec3Position(position);
	}
}