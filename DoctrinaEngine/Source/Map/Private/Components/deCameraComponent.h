#ifndef _DECAMERACOMPONENT_H_
#define _DECAMERACOMPONENT_H_

#include "Map/Private/deMapComponent.h"
#include "Math/Public/deMath.h"

namespace de::map
{
    class CameraComponent : public de::map::MapComponent
    {
    public:
        ~CameraComponent() override;

        void Draw() override;
        void Update(float dT) override;

        math::Vector3 Position;
        math::Vector3 Rotation;
        math::Matrix ViewMatrix;
    };
}

#endif // !_DECAMERACOMPONENT_H_