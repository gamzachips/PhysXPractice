#include "pch.h"
#include "Camera.h"
#include "InputManager.h"

void Camera::Update(float deltaTime)
{
    if (_inputVector.LengthSquared() > 0.0f)
    {
        _position += _inputVector * _speed * deltaTime;
        _inputVector = SimpleMath::Vector3::Zero;
    }
    if (_rotateInputVector.LengthSquared() > 0.0f)
    {
        _rotation += _rotateInputVector * _rotateSpeed * deltaTime;
        _rotateInputVector = SimpleMath::Vector3::Zero;
    }

    _worldTransform = SimpleMath::Matrix::CreateFromYawPitchRoll(_rotation) *
        SimpleMath::Matrix::CreateTranslation(_position);

    SimpleMath::Vector3 eye = _worldTransform.Translation();
    SimpleMath::Vector3 target = _worldTransform.Translation() + GetForward();
    SimpleMath::Vector3 up = _worldTransform.Up();
    _view = XMMatrixLookAtLH(eye, target, up);

    _projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(_fieldOfView), _aspectRatio, _nearPlane, _farPlane);
}

void Camera::GetViewMatrix(OUT Matrix& view)
{
    view = _view;
}

void Camera::GetProjectionMatrix(OUT Matrix& projection)
{
    projection = _projection;
}

void Camera::AddInputVector(const SimpleMath::Vector3 input)
{
    _inputVector += input;
    _inputVector.Normalize();
}

void Camera::AddRotateInputVector(const SimpleMath::Vector3 input)
{
    _rotateInputVector += input;
}
