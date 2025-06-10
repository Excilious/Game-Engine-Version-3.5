#include "Lighting.h"

Lighting::Lighting() 
{};

void Lighting::CreateLight(glm::vec3 Colours)
{
    this->Colours = Colours;
    this->Light = LIGHT_DIRECT;
}