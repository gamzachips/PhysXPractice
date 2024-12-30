#pragma once
class Transform
{
public:
	void UpdateTransform();
public:
	void Rotate(Vector4 angle) {
		_rotation.x += angle.x;
		_rotation.y += angle.y;
		_rotation.z += angle.z;
		_rotation.w += angle.w;
	}
	void SetPosition(Vector4 pos) { _translation = pos; }
	void SetSize(Vector4 size) { _size = size; }

	Transform* parent = nullptr;

	Vector4 _size = { 1.f, 1.f, 1.f, 1.f };
	Vector4 _rotation{};
	Vector4 _translation{};
	Matrix _transform;
};

