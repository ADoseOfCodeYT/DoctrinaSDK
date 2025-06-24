#ifndef _DEMAPCOMPONENT_H_
#define _DEMAPCOMPONENT_H_

namespace de::map 
{
    class MapComponent
    {
    public:
        virtual ~MapComponent() = default;

        virtual void Draw() = 0;
        virtual void Update(float dT) = 0;
    };
}

#endif // !_DEMAPCOMPONENT_H_