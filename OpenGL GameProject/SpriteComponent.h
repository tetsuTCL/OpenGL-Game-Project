#pragma once
#include"Component.h"
#include"SDL.h"
#include<glew.h>

class SpriteComponent : public Component
{
public:
	SpriteComponent(class Actor* owner, int drawOrder = 100);
	~SpriteComponent();
	virtual void Draw(SDL_Renderer* renderer);
	virtual void Draw(class Shader* shader);
	virtual void SetSDLTexture(SDL_Texture* sdltexture);
	virtual void SetTexture(class Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTextureWidth() const{ return mTextureWidth; }
	int GetTextureheight() const { return mTextureHeight; }

protected:

	SDL_Texture* sdlTexture;
	class Texture* mTexture;
	int mDrawOrder;

	int mTextureWidth;
	int mTextureHeight;
};
