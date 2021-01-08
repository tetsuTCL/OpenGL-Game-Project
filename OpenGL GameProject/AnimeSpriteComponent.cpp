#include"AnimeSpriteComponent.h"
#include"SpriteComponent.h"
#include"Math.h"

////////
AnimeSpriteComponent::AnimeSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrentFrame(0.0f)
	, mAnimeFPS(24.0f)
{

}
////////

void AnimeSpriteComponent::SetAnimeTextures(const std::vector<SDL_Texture*>& textures)
{
	/////////////////////
	mAnimeTextures = textures;
	if (mAnimeTextures.size() > 0)
	{
		mCurrentFrame = 0.0f;
		SetSDLTexture(mAnimeTextures[0]);
	}
	/////////////////////
}

void AnimeSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimeTextures.size() > 0)
	{
		mCurrentFrame += mAnimeFPS * deltaTime;

		while (mCurrentFrame >= mAnimeTextures.size())
		{
			mCurrentFrame -= mAnimeTextures.size();
		}

		SetSDLTexture(mAnimeTextures[static_cast<int>(mCurrentFrame)]);
	}
}