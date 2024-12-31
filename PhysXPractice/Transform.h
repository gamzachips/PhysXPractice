#pragma once
class Transform
{
public:
	void UpdateTransform();
	void UpdateFromPxTransform(PxTransform pxTransform);
public:
	void Rotate(Vector4 angle) {
		_rotation.x += angle.x;
		_rotation.y += angle.y;
		_rotation.z += angle.z;
		_rotation.w += angle.w;
	}
	void SetPosition(Vector4 pos) { _translation = pos; }
	void SetSize(Vector4 size) { _size = size; }

	Vector3 GetWorldPosition() const {
		return { _transform._41, _transform._42, _transform._43};
	}

	Transform* parent = nullptr;
	PxTransform _pxTransform; 
	Vector4 _size = { 1.f, 1.f, 1.f, 1.f };
	Vector4 _rotation{};
	Vector4 _translation{};
	Matrix _transform;
};

