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

   
    //Update PxTransform 

    PxVec3 position(_translation.x, _translation.y, _translation.z);

    Quaternion rotQut = Quaternion::CreateFromYawPitchRoll(_rotation.y, _rotation.x, _rotation.z);
    PxQuat pxRot = PxQuat(rotQut.x, rotQut.y, rotQut.z, rotQut.w);

    _pxTransform = PxTransform(position, pxRot);
}

void Transform::UpdateFromPxTransform(PxTransform pxTransform)
{
    _pxTransform = pxTransform;
    PxTransform localTransform = _pxTransform;
    if (parent)
    {
        PxTransform parentInverse = parent->_pxTransform.getInverse();
        localTransform = parentInverse.transform(_pxTransform);
    }
   
    _translation = Vector4(localTransform.p.x, localTransform.p.y, localTransform.p.z, 0.f);
    
    Quaternion quaternion(localTransform.q.x, localTransform.q.y, localTransform.q.z, localTransform.q.w);
    Vector3 rotation = quaternion.ToEuler();

    _rotation = Vector4(rotation.x, rotation.y, rotation.z, 0.f);
}

