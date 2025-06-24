#ifndef _DEMATHALIASES_H_
#define _DEMATHALIASES_H_

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

// Header file which aliases DirectX types
// It is included in deMath.h

namespace de::math
{
    using Vector = DirectX::XMVECTOR;

    using Vector2 = DirectX::XMFLOAT2;

    using Vector3 = DirectX::XMFLOAT3;

    using Matrix = DirectX::XMMATRIX;

    // VECTOR

    inline Vector Vector3TransformCoord(Vector lookAtVec, Matrix rotMatrix)
    {
        return DirectX::XMVector3TransformCoord(lookAtVec, rotMatrix);
    }

    inline Vector VectorAdd(Vector posVec, Vector lookAtVec)
    {
        return DirectX::XMVectorAdd(posVec, lookAtVec);
    }

    // MATRIX

    inline Matrix IdentityMatrix()
    {
        return DirectX::XMMatrixIdentity();
    }

    inline Matrix MatrixPerspectiveFovLH(float fieldOfView, float aspectRatio, float nearZ, float farZ)
    {
        return DirectX::XMMatrixPerspectiveFovLH(fieldOfView, aspectRatio, nearZ, farZ);
    }

    inline Matrix MatrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
    {
        return DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
    }

    inline Matrix MatrixRotationRollPitchYaw(float pitch, float yaw, float roll)
    {
        return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    }

    inline Matrix MatrixLookAtLH(Vector posVec, Vector focusVec, Vector upVec)
    {
        return DirectX::XMMatrixLookAtLH( posVec,  focusVec,  upVec);
    }
}

#endif // ! _DEMATHALIASES_H_