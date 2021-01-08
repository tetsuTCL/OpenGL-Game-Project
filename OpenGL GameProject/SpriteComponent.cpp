#include"SpriteComponent.h"
#include"Texture.h"
#include"Shader.h"
#include"Actor.h"
#include"Game.h"
#include"Renderer.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder)
	:Component(owner)
{
	mTexture = nullptr;
	sdlTexture = nullptr;
	mDrawOrder = drawOrder;
	mTextureWidth = 0;
	mTextureHeight = 0;
	mOwner->GetGame()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetGame()->RemoveSprite(this);
}

void SpriteComponent::SetSDLTexture(SDL_Texture* sdltexture)
{
	sdlTexture = sdltexture;

	SDL_QueryTexture(sdltexture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}

void SpriteComponent::SetTexture(Texture* texture)
{
	mTexture = texture;
	mTextureWidth = texture->GetWidth();
	mTextureHeight = texture->GetHeight();
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (sdlTexture)
	{
		SDL_Rect r;
		//Scale
		r.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
		//Central position
		r.x = static_cast<int>(mOwner->GetPosition().x - r.w / 2);
		r.x = static_cast<int>(mOwner->GetPosition().y - r.h / 2);

		SDL_RenderCopyEx(renderer,
			sdlTexture, nullptr, &r,
			-Math::ToDegrees(mOwner->GetRotation()),
			nullptr, SDL_FLIP_NONE);		
	}
	
}

void SpriteComponent::Draw(Shader* shader)
{
	if (mTexture)
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTextureWidth),
			static_cast<float>(mTextureHeight), 1.0f);

		Matrix4 world = scaleMat * mOwner->GetWorldTransform();

		shader->SetMatrixUniform("uWorldTransform", world);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		mTexture->SetActive();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
}