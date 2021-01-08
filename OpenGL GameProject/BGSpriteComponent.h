#pragma once
#include"SpriteComponent.h"
#include"Math.h"
#include<vector>

class BGSpriteComponent : public SpriteComponent
{
public:
	BGSpriteComponent(Actor* owner, int drawOrder = 100);
	void Update(float deltaTime)override;
	void Draw(SDL_Renderer* renderer)override;

	void SetBGTextures(const std::vector<SDL_Texture*>& textures);
	void SetScreenSize(const Vector2& size) { mScreenSize = size; }
	void SetScrollSpeed(float speed) { mScrollSpeed = speed; }
	float GetScrollSpeed()const { return mScrollSpeed; }

private:

	struct BGTexture
	{
		BGTexture() { mSDLTexture = nullptr; }
		SDL_Texture* mSDLTexture;
		Vector2 mOffset;
	};
	std::vector<BGTexture> mBGTextures;
	Vector2 mScreenSize;
	float mScrollSpeed;
};
