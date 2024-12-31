#pragma once

#include "D3DRenderer.h"
#include "Scene.h"

class InputManager;
class TimeManager;
class ResourceManager;
class PhysicsManager;


class Game
{
public:
	Game();
	~Game();
	void Init();
	void Update();
	void Render();

	static D3DRenderer GetRenderer() { return mRenderer; }
	static InputManager* GetInputManager() { return mInputManager; }
	static TimeManager* GetTimeManager() { return mTimeManager; }
	static ResourceManager* GetResourceManager() { return mResourceManager; }
	static PhysicsManager* GetPhysicsManager() { return mPhysicsManager; }

	Scene* GetNowScene() { return mNowScene; }
	LightData GetLightData() { return mLightData; }

	template<typename T>
	void ChangeScene()
	{
		if (mNowScene)
			delete mNowScene;
		mNowScene = new T;
		mNowScene->Init(mRenderer.GetDevice());
	}

private:
	static D3DRenderer mRenderer;
	static InputManager* mInputManager;
	static TimeManager* mTimeManager;
	static ResourceManager* mResourceManager;
	static PhysicsManager* mPhysicsManager;

	LightData mLightData;
	Scene* mNowScene = nullptr;
};

