#ifndef __LIGHTING_HEADER__
#define __LIGHTING_HEADER__

#include "../Model.h"

typedef enum
{  
    LIGHT_POINT,
    LIGHT_DIRECT,
    LIGHT_SPOT
} LightType; 

class Lighting: public Model
{
    public:
        Lighting();
        void CreateLight(glm::vec3 Colours);

        float       Brightness;
        float       Gloss;
        float       Transparency;
        glm::vec3   Colours;
        LightType   Light;
    private:
        
};

#endif