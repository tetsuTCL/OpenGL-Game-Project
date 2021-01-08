#include"SDL.h"
#include"SDL_image.h"
#include"Game.h"
#include<algorithm>
#include"Renderer.h"
#include"Actor.h"
#include"SpriteComponent.h"
#include"MeshComponent.h"
#include"CameraActor.h"


Game::Game()
	:mSDLRenderer(nullptr),
	mIsRunning(true),
	mTicksCount(0),
	mUpdatingActors(false),
	mContent(nullptr),
	mSpriteShader(nullptr),
	mSpriteVerts(nullptr),
	mRenderer(nullptr),
	mCameraActor(nullptr)
{

}

bool Game::Initialize() {

	//int sdlresult = SDL_Init(SDL_INIT_VIDEO);
	//if (sdlresult != 0) {
	//	SDL_Log("SDL Initialize Failure : %s", SDL_GetError());
	//	return false;
	//}
	///////Add OpenGL profile
	/////
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	/////
	//mWindow = SDL_CreateWindow(
	//	"OpenGL_Project",
	//	100,
	//	50,
	//	1024,
	//	768,
	//	SDL_WINDOW_OPENGL ////
	//);
	//mContent = SDL_GL_CreateContext(mWindow);
	////Initialize GLEW
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	SDL_Log("Failed to initialize GLEW");
	//	return false;
	//}
	//glGetError();
	//if (!mWindow) {
	//	SDL_Log("Window Generate Failure : %s", SDL_GetError());
	//	return false;
	//}
	//mSDLRenderer = SDL_CreateRenderer(
	//	mWindow,
	//	-1,
	//	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	//if (!mSDLRenderer)
	//{
	//	SDL_Log("Failed to create renderer: %s", SDL_GetError());
	//	return false;
	//}
	//if (IMG_Init(IMG_INIT_PNG) == 0)
	//{
	//	SDL_Log("Unable to initialize SDL image: %s", SDL_GetError());
	//	return false;
	//}

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	//Create the renderer
	mRenderer = new Renderer(this);
	if (!mRenderer->Initialize(1024.0f, 768.0f))
	{
		SDL_Log("Failed to initialize renderer");
		delete mRenderer;
		mRenderer = nullptr;
		return false;
	}

	mTicksCount = SDL_GetTicks();

	LoadData();

	return true;
}

void Game::RunLoop() {
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput() {

	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		    case SDL_QUIT:
			   mIsRunning = false;
			   break;
		}
	}
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}

	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}

}

void Game::LoadData()
{
	Actor* actor = new Actor(this);
	SpriteComponent* spriteComponent = new SpriteComponent(actor);
	//spriteComponent->SetTexture(LoadTexture("")); Load Texture files

	//Create actors
	actor->SetVec3Position(Vector3(200.0f, 75.0f, 0.0f));
	actor->SetScale(100.0f);
	Quaternion q(Vector3::UnitY, -Math::PiOver2);
	q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	actor->SetQRotation(q);
	MeshComponent* mc = new MeshComponent(actor);
	mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

	actor = new Actor(this);
	actor->SetVec3Position(Vector3(200.0f, -75.0f, 0.0f));
	actor->SetScale(3.0f);
	mc = new MeshComponent(actor);
	mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

	actor = new Actor(this);
	actor->SetVec3Position(Vector3(500.0f, 75.0f, -80.0f));
	actor->SetScale(1.0f);
	Quaternion qua(Vector3::UnitX, +Math::PiOver2);
	qua = Quaternion::Concatenate(qua, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
	actor->SetQRotation(qua);
	mc = new MeshComponent(actor);
	mc->SetMesh(mRenderer->GetFBXMesh("Assets/chara02.fbx"));

	// Setup lights
	mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
	DirectionalLight& dir = mRenderer->GetDirectionalLight();
	dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
	dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
	dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

	// Camera actor
	mCameraActor = new CameraActor(this);
}

void Game::UnloadData()
{
	//Delete actors
	while (!mActors.empty())
	{
		delete mActors.back();
	}

	//Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
	//Delete actors
    //Because ~Actor calls RemoveActor, have to use a different style loop
	if (mRenderer)
	{
		mRenderer->UnloadData();
	}
} 

void Game::AddActor(Actor* actor)
{
	if (mUpdatingActors) {
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		//Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		//Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::UpdateGame() {

	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	float deltatime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	mTicksCount = SDL_GetTicks();

	if (deltatime > 0.05f) {
		deltatime = 0.05f;
	}

	mUpdatingActors = true; //Update all actors

	for (auto actor : mActors)
	{
		actor->Update(deltatime);
	}
	mUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		pending->ComputeWorldTransform();
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActors;

	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::State::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}
}

SDL_Texture* Game::LoadTexture(const char* fileName)
{
	SDL_Surface* surf = IMG_Load(fileName);
	if (!surf)
	{
		SDL_Log("Failed to load texture file %s", fileName);
		return nullptr;
	}

	SDL_Texture* text = SDL_CreateTextureFromSurface(mSDLRenderer, surf);
	SDL_FreeSurface(surf);
	if (!text)
	{
		SDL_Log("Failed to convert surface to texture for %s", fileName);
		return nullptr;
	}
	return text;
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); iter++)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

//Draw
void Game::GenerateOutput() {

	//glClearColor(0.86f, 0.86f, 0.86f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//glEnable(GL_BLEND); //srcFactor = srcAlpha
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //dstFactor = (1 - srcAlpha)
	//SDL_GL_SwapWindow(mWindow);

	//SDL_SetRenderDrawColor(
	//	mSDLRenderer,
	//	0x0A, 0xE0, 0xBA, //Background RGB
	//	255 //Alpha value
	//);
	//SDL_RenderClear(mSDLRenderer);	//Clear Draw Screen
	//SDL_SetRenderDrawColor(mSDLRenderer, 255, 255, 255, 255);
	//// Draw all sprite components
	//for (auto sprite : mSprites)
	//{
	//	sprite->Draw(mSDLRenderer);
	//}
	//// Swap front buffer and back buffer
	//SDL_RenderPresent(mSDLRenderer);

	mRenderer->Draw();
}

void Game::ShutDown() {
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mSDLRenderer);
	UnloadData();
	if (mRenderer)
	{
		mRenderer->Shutdown();
	}
	SDL_Quit();
}


