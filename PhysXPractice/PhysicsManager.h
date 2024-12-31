#pragma once

static PxDefaultErrorCallback gDefaultErrorCallback;
static PxDefaultAllocator gDefaultAllocatorCallback;


class PhysicsManager
{
public:

	~PhysicsManager();

public:
	void Initialize();

	PxPhysics* GetPhysics() const { return mPhysics; }
	PxMaterial* GetDefaultMaterial() { return mDefaultMaterial; }
private:

	PxFoundation* mFoundation;
	bool  recordMemoryAllocations = true;
	PxPvd* mPvd;
	PxPhysics* mPhysics;
	PxMaterial* mDefaultMaterial;
};

