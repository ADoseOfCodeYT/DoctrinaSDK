#ifndef _DEMATHALIASES_H_
#define _DEMATHALIASES_H_

#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXCollision.h>

// Header file which aliases DirectX types
// It is included in deMath.h

namespace de::Math
{
    using Vector2 = DirectX::XMFLOAT2;
    using Vector3 = DirectX::XMFLOAT3;

    using Matrix = DirectX::XMMATRIX;

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
}

#endif // ! _DEMATHALIASES_H_