#pragma once
#include"Math.h"
#include<vector>
#include<cstdint>

class Actor
{
public:

	enum class State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();
    void Update(float deltaTime);
	void UpdateComponents(float deltaTime);
	virtual void UpdateActor(float deltaTime);
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

	// ProcessInput function called from Game (not overridable)
	void ProcessInput(const uint8_t* keyState);
	// Any actor-specific input code (overridable)
	virtual void ActorInput(const uint8_t* keyState);
	
	// Getters/setters

	void SetPosition(const Vector2& position) { m2Position = position; }
	const Vector2& GetPosition() const { return m2Position; }
	
	void SetScale(float scale) { mScale = scale; }
	float GetScale() const { return mScale; }

	void SetRotation(float rotation) { m2Rotation = rotation; }
	float GetRotation() const { return m2Rotation; }

	void SetState(State state) { mState = state; }
	State GetState()const { return mState; }

	//3D
	void SetVec3Position(const Vector3& pos) { m3Position = pos; mRecomputeWorldTransform = true; }
	const Vector3& GetVec3Position() const { return m3Position; }
	Vector3 GetForward() const { return Vector3::Transform(Vector3::UnitX, m3Rotation); }
	
	void SetQRotation(const Quaternion& rotation) { m3Rotation = rotation;  mRecomputeWorldTransform = true; }
	const Quaternion& GetQRotation() const { return m3Rotation; }

	const Matrix4& GetWorldTransform() const { return mWorldTransform; }
	void ComputeWorldTransform();

	class Game* GetGame(){ return mGame; }

private:
	State mState;
	//For 2D environment
	Vector2 m2Position;
	float mScale;
	float m2Rotation;

	//For 3D environment
	Vector3 m3Position;
	Quaternion m3Rotation;
	Matrix4 mWorldTransform;
	bool mRecomputeWorldTransform;

	std::vector<class Component*> mComponents;
	class Game* mGame;
	
};
