#include "pch.h"
#include "Game.h"
#include "InputManager.h"
#include "TimeManager.h"
#include "ResourceManager.h"
#include "Scene.h"

D3DRenderer Game::mRenderer;
InputManager* Game::mInputManager = nullptr;
TimeManager* Game::mTimeManager = nullptr;
ResourceManager* Game::mResourceManager = nullptr;
PhysicsManager* Game::mPhysicsManager = nullptr;

Game::Game()
{
	mInputManager = new InputManager;
	mTimeManager = new TimeManager;
	mResourceManager = new ResourceManager;
	mPhysicsManager = new PhysicsManager;
}

Game::~Game()
{
	delete mInputManager;
	delete mTimeManager;
	delete mResourceManager;
}

void Game::Init()
{
	mRenderer.Init();
	mInputManager->Init();
	mTimeManager->Init();
	mResourceManager->Init();

	//Init
	mNowScene->Init(mRenderer.GetDevice());
}

void Game::Update()
{
	mInputManager->Update();
	mTimeManager->Update();
	//Update
	mNowScene->Update(mTimeManager->GetDeltaTime());

}

void Game::Render()
{
	mRenderer.RenderBegin();
	//Render
	mNowScene->Render(mRenderer.GetDeviceContext());
	mRenderer.RenderEnd();

}
