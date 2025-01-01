#pragma once

class Object;
class Camera;
class DirectionalLight;

class Scene
{
public:
	Scene();
	virtual ~Scene();
	virtual void Init(ComPtr<ID3D11Device> device);
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);
	virtual void Render(ComPtr<ID3D11DeviceContext> dc);

	Camera* GetCamera() { return mCamera; }

	void AddObject(Object* object) { mObjects.push_back(object); }
	void AddPxActor(PxActor* actor) { mPxScene->addActor(*actor); }

	PxScene* GetPxScene() { return mPxScene; }
protected:

	void MoveCamera();
	void RotateCamera();

	Camera* mCamera;
	std::vector<Object*> mObjects;
	PxScene* mPxScene;
	class PhysicsEvent* mEventCallback;
};

