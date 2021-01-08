#include"BGSpriteComponent.h"
#include"Actor.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder),
	mScrollSpeed(0.0f)
{
}

void BGSpriteComponent::SetBGTextures(const std::vector<SDL_Texture*>& textures)
{
	int count = 0;
	for (auto tex : textures)
	{
		BGTexture temp;
		temp.mSDLTexture = tex;
		// Each texture is screen width in offset
		temp.mOffset.X = count * mScreenSize.X;
		temp.mOffset.Y = 0;
		mBGTextures.emplace_back(temp);
		count++;
	}
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);
	for (auto& bg : mBGTextures)
	{
		bg.mOffset.X += mScrollSpeed * deltaTime;

		if (bg.mOffset.X < -mScreenSize.X)
		{
			bg.mOffset.X = (mBGTextures.size() - 1) * mScreenSize.X - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
	// Draw each background texture
	for (auto& bg : mBGTextures)
	{
		SDL_Rect r;
		// Assume screen size dimensions
		r.w = static_cast<int>(mScreenSize.X);
		r.h = static_cast<int>(mScreenSize.Y);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(mOwner->GetPosition().X - r.w / 2 + bg.mOffset.X);
		r.y = static_cast<int>(mOwner->GetPosition().Y - r.h / 2 + bg.mOffset.Y);

		// Draw this background
		SDL_RenderCopy(renderer,
			bg.mSDLTexture,
			nullptr,
			&r
		);
	}
}