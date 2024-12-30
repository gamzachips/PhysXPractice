#include "pch.h"
#include "Transform.h"

void Transform::UpdateTransform()
{
    XMVECTOR scale = XMLoadFloat4(&_size);
    XMVECTOR rotation = XMLoadFloat4(&_rotation);
    XMVECTOR translation = XMLoadFloat4(&_translation);

    XMMATRIX scaleMatrix = XMMatrixScalingFromVector(scale);
    XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYawFromVector(rotation);
    XMMATRIX translationMatrix = XMMatrixTranslationFromVector(translation);

    _transform = scaleMatrix * rotationMatrix * translationMatrix;

    if (parent)
        _transform = _transform * parent->_transform;

}

