#ifndef _EDITOR_H_
#define _EDITOR_H_

#include "DoctrinaEngine.h"

class Editor : public de::Application
{
public:
    void Initialize() override;

    void FixedUpdate() override;
};


#endif //_EDITOR_H_