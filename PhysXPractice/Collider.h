#pragma once


class Collider
{
public:
	Collider();
	virtual ~Collider() {}
public:
	virtual void Init() abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void Render(ComPtr<ID3D11DeviceContext> dc) abstract;

	void SetLocalPosition(Vector3 pos);
	void SetRotation(Vector3 rotation);

	void SetIsTrigger(bool isTrigger);
	bool GetIsTrigger() { return mIsTrigger; }

private:
	bool mIsTrigger = false;

protected:
	PxShape* mShape = nullptr;

	friend class RSObject;
};


