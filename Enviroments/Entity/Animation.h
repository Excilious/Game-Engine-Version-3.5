#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "../Dependancies/jsonimport/json.h"
#include "../Model.h"
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

class AnimationInstance
{
    public:
        struct Keyframe
        {
            float       Time;
            glm::vec3   Translation;
            glm::quat   Rotation;
            glm::vec3   Scale;
        };

        struct AnimationKeyframe {std::vector<Keyframe> Keyframes;};

        std::unordered_map<int,AnimationKeyframe> Channels;
        void AddKeyframe(int Node,const Keyframe& NewKeyframe);
        glm::mat4 GetNodeTransform(int Node,float CurrentTime);
};

class Animation: public Model
{
    public:
        Animation();
        int AnimationDelta;
        
        void LoadSkeleton();
        void RenderAnimations(int StartNode);
    private:
        AnimationInstance   NewAnimationInstance;
        Json                NewJson;
        template <typename T>
        std::vector<T>  DecodeBuffer(const Json& Buffer,const Json& BufferView,const Json& Accessor);
        std::vector<uint8_t> DecodeBase64(const std::string& Base64);
};

#endif