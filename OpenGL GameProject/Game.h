#pragma once
#include"SDL.h"
#include"glew.h"
#include<vector>
#include<unordered_map>
#include<string>
#include"Math.h"

#include"VertexArray.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void ShutDown();
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);
	SDL_Texture* LoadTexture(const char* file);
	class Renderer* GetRenderer() { return mRenderer; }

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	SDL_Renderer* mSDLRenderer;
	SDL_GLContext mContent;
	
	bool mIsRunning;
	bool mUpdatingActors;
	Uint32 mTicksCount;

	VertexArray* mSpriteVerts;
    class Shader* mSpriteShader;
	class Renderer* mRenderer;
	class CameraActor* mCameraActor;

	//All actors in the game
	std::vector<Actor*> mActors;
	//Any pending actors
	std::vector<Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
};
