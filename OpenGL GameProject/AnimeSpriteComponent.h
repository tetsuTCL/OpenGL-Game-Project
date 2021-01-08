#pragma once
#include<vector>
#include"Actor.h"
#include"SpriteComponent.h"

class AnimeSpriteComponent : public SpriteComponent
{
public:
	AnimeSpriteComponent(Actor* owner, int drawOrder = 100);
	void Update(float deltaTime) override;
	void SetAnimeTextures(const std::vector<SDL_Texture*>& textures);
	void SetAnimeFPS(float fps) { mAnimeFPS = fps; }
	float GetAnimeFPS() const { return mAnimeFPS; }
private:
	std::vector<SDL_Texture*> mAnimeTextures;
	float mCurrentFrame;
	float mAnimeFPS;
};
