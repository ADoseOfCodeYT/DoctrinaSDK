#include "Map/Private/Components/deCameraComponent.h"
#include "Math/Private/deMathAliases.h"

using namespace de::math;

namespace de::map
{
    CameraComponent::~CameraComponent()
    {
        Position = Vector3(0.0f, 0.0f, 0.0f);
        Rotation = Vector3(0.0f, 0.0f, 0.0f);
    }

    void CameraComponent::Draw() 
    {
        Vector3 up, position, lookAt;
        Vector upVector, positionVector, lookAtVector;
        float yaw, pitch, roll;
        Matrix rotationMatrix;


        // Setup the vector that points upwards.
        up.x = 0.0f;
        up.y = 1.0f;
        up.z = 0.0f;

        // Load it into a XMVECTOR structure.
        upVector = XMLoadFloat3(&up);

        // Setup the position of the camera in the world.
        position.x = Position.x;
        position.y = Position.y;
        position.z = Position.z;

        // Load it into a XMVECTOR structure.
        positionVector = XMLoadFloat3(&position);

        // Setup where the camera is looking by default.
        lookAt.x = 0.0f;
        lookAt.y = 0.0f;
        lookAt.z = 1.0f;

        // Load it into a XMVECTOR structure.
        lookAtVector = XMLoadFloat3(&lookAt);

        // Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
        pitch = Rotation.x * 0.0174532925f;
        yaw   = Rotation.y * 0.0174532925f;
        roll  = Rotation.z * 0.0174532925f;

        // Create the rotation matrix from the yaw, pitch, and roll values.
        rotationMatrix = MatrixRotationRollPitchYaw(pitch, yaw, roll);

        // Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
        lookAtVector = Vector3TransformCoord(lookAtVector, rotationMatrix);
        upVector = Vector3TransformCoord(upVector, rotationMatrix);

        // Translate the rotated camera position to the location of the viewer.
        lookAtVector = VectorAdd(positionVector, lookAtVector);

        // Finally create the view matrix from the three updated vectors.
        ViewMatrix = MatrixLookAtLH(positionVector, lookAtVector, upVector);
    }

    void CameraComponent::Update(float dT)
    {

    }

}