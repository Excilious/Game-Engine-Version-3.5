#include "Animation.h"
#include "../Shader.h"
#include "../Console.h"

Animation::Animation() {}

std::vector<uint8_t> Animation::DecodeBase64(const std::string& Base64)
{
    std::string Base64Values = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::vector<uint8_t> Decoded;
    int Value   = 0;
    int Valueb  = -8;

    for (int BaseIndex = 0; BaseIndex < Base64.size(); BaseIndex++)
    {
        if (Base64Values.find(Base64[BaseIndex]) == std::string::npos)
        {break;}
        Value   = (Value << 6) + Base64Values.find(Base64[BaseIndex]);
        Valueb += 6;
        if (Valueb >= 0)
        {
            Decoded.push_back((Value>>Valueb)&0xFF);
            Valueb -= 8;
        }
    }
    return Decoded;
}

template <typename T>
std::vector<T> Animation::DecodeBuffer(const Json& Buffer,const Json& BufferView,const Json& Accessor)
{
    std::cout << "DA" << "\n";
    std::string URI = Buffer["uri"];
    std::vector<uint8_t> BinaryData;

    std::cout << "DB" << "\n";
    if (URI.find("data:") == 0)
    {BinaryData = this->DecodeBase64(URI.substr(URI.find(",")+1));}
    else 
    {
        std::ifstream Input(URI,std::ios::binary);
        BinaryData = std::vector<uint8_t>(std::istreambuf_iterator<char>(Input),std::istreambuf_iterator<char>());
    }
    std::cout << "DC" << "\n";
    size_t ByteOffset = BufferView.value("byteOffset", 0);
    size_t ByteLength = BufferView["byteLength"];
    const uint8_t* Start = BinaryData.data() + ByteOffset;
    size_t Count = Accessor["count"];
    size_t Stride = Accessor.contains("byteStride") ? Accessor["byteStride"].get<size_t>() : sizeof(T);

    std::cout << "DD" << "\n";
    std::vector<T> Result;

    // if (ByteOffset + ByteLength > BinaryData.size()) {
    //     size_t maxAllowedLength = BinaryData.size() - ByteOffset;
    //     throw std::runtime_error("Warning: ByteOffset + ByteLength exceeds buffer size. Adjusting ByteLength to fit available data.");
    //     ByteLength = maxAllowedLength;
    // }

    size_t requiredSize = Count * Stride;
    if (requiredSize > ByteLength)                                  {throw std::runtime_error("Error: Accessor count * stride exceeds buffer view length.");}
    if (ByteOffset + ByteLength > BinaryData.size())                {throw std::runtime_error("ByteOffset + ByteLength exceeds buffer size.");}
    if (ByteOffset + Count * Stride > BinaryData.size())            {throw std::runtime_error("Access out of bounds: Count * Stride exceeds buffer size.");}
    if (reinterpret_cast<uintptr_t>(Start) % alignof(T) != 0)       {throw std::runtime_error("Start pointer is not aligned for type T.");}

    for (size_t Index = 0; Index < Count; ++Index) {
        const uint8_t* Current = Start + Index * Stride;
        if (Current + sizeof(T) > BinaryData.data() + BinaryData.size()) {throw std::runtime_error("Out-of-bounds memory access during iteration.");}
        T Value;
        std::memcpy(&Value, Current, sizeof(T));
        Result.push_back(Value);
    }
    return Result;
}

void AnimationInstance::AddKeyframe(int Node,const Keyframe& NewKeyframe)
{this->Channels[Node].Keyframes.push_back(NewKeyframe);}

glm::mat4 AnimationInstance::GetNodeTransform(int Node,float CurrentTime)
{
    if (this->Channels.find(Node) == this->Channels.end()) {return glm::mat4(1.0f);}
    
    const std::vector<Keyframe>& KeyframeList = this->Channels[Node].Keyframes;
    if (KeyframeList.empty()) {return glm::mat4(1.0f);}

    const Keyframe* Previous    = nullptr;
    const Keyframe* Next        = nullptr;

    for (size_t Index = 0; Index < KeyframeList.size()-1; Index++)
    {
        if (KeyframeList[Index].Time <= CurrentTime && KeyframeList[Index+1].Time > CurrentTime)
        {
            Previous    = &KeyframeList[Index];
            Next        = &KeyframeList[Index+1];
            break;
        }
    }

    if (!Previous || !Next) {return glm::mat4(1.0f);}
    float Factor            = (CurrentTime-Previous->Time) / (Next->Time - Previous->Time);
    glm::vec3 Translation   = glm::mix(Previous->Translation,Next->Translation,Factor);
    glm::quat Rotation      = glm::slerp(Previous->Rotation,Next->Rotation,Factor);
    glm::vec3 Scale         = glm::mix(Previous->Scale,Next->Scale,Factor);
    glm::mat4 Transform     = glm::translate(glm::mat4(1.0f),Translation) * glm::mat4_cast(Rotation) * glm::scale(glm::mat4(1.0f),Scale);

    return Transform;
}

void Animation::LoadSkeleton()
{
    try
    {
        std::string Contents    = GetShaderContents(this->NewFile);
        this->NewJson           = Json::parse(Contents);
        
        if (this->NewJson.find("animations") == this->NewJson.end()) {throw std::runtime_error("Animation doesnt exist.");}
        else 
        {
            for (int AnimationIndex = 0; AnimationIndex < this->NewJson["animations"].size(); AnimationIndex++)
            {
                for (int ChannelIndex = 0; ChannelIndex < this->NewJson["animations"][AnimationIndex]["channels"].size(); ChannelIndex++)
                {
                    std::cout << "A" << "\n";
                    Json ChannelInstance            = this->NewJson["animations"][AnimationIndex]["channels"][ChannelIndex];
                    Json Target                     = ChannelInstance["target"];
                    int SamplerIndex                = ChannelInstance["sampler"];
                    int NodeIndex                   = Target["node"];
                    std::string Path                = Target["path"];

                    std::cout << "B" << "\n";
                    Json Sampler                    = this->NewJson["animations"][AnimationIndex]["samplers"][SamplerIndex];
                    int InputAccessorIndex          = Sampler["input"];
                    int OutputAccessorIndex         = Sampler["output"];

                    Json InputAccessor              = this->NewJson["accessors"][InputAccessorIndex];
                    Json InputBufferView            = this->NewJson["bufferViews"][InputAccessor["bufferView"].get<int>()];
                    Json InputBuffer                = this->NewJson["buffers"][InputBufferView["buffer"].get<int>()];
                    std::vector<float> Times        = this->DecodeBuffer<float>(InputBuffer,InputBufferView,InputAccessor);

                    Json OutputAccessor             = this->NewJson["accessors"][OutputAccessorIndex];
                    Json OutputBufferView           = this->NewJson["bufferViews"][OutputAccessor["bufferView"].get<int>()];
                    Json OutputBuffer               = this->NewJson["buffers"][OutputBufferView["buffer"].get<int>()];

                    std::cout << "E" << "\n";
                    if (Path == "translation")
                    {
                        std::vector<glm::vec3> Translations = this->DecodeBuffer<glm::vec3>(OutputBuffer,OutputBufferView,OutputAccessor);
                        for (size_t Index = 0; Index < Times.size(); Index++)
                        {
                            AnimationInstance::Keyframe  NewKeyFrame{Times[Index],Translations[Index],glm::quat(),glm::vec3(1.0f)};
                            this->NewAnimationInstance.AddKeyframe(NodeIndex,NewKeyFrame);
                        }
                    }
                    else if (Path == "rotation")
                    {
                        std::vector<glm::quat> Rotations = this->DecodeBuffer<glm::quat>(OutputBuffer,OutputBufferView,OutputAccessor);
                        for (size_t Index = 0; Index < Times.size(); Index++)
                        {
                            AnimationInstance::Keyframe NewKeyFrame{Times[Index],glm::vec3(0.0f),Rotations[Index],glm::vec3(1.0f)};
                            this->NewAnimationInstance.AddKeyframe(NodeIndex,NewKeyFrame);
                        }
                    }
                    else if (Path == "scale")
                    {
                        std::vector<glm::vec3> Scales = this->DecodeBuffer<glm::vec3>(OutputBuffer,OutputBufferView,OutputAccessor);
                        for (size_t Index = 0; Index < Times.size(); Index++)
                        {
                            AnimationInstance::Keyframe NewKeyFrame{Times[Index],glm::vec3(0.0f),glm::quat(),Scales[Index]};
                            this->NewAnimationInstance.AddKeyframe(NodeIndex,NewKeyFrame);
                        }
                    }
                    std::cout << "F" << "\n";
                }
            }
        }
    }
    catch (const std::runtime_error& e)
    {SysPrint(e.what(),3);}
}

void Animation::RenderAnimations(int StartNode)
{
    try
    {
        this->InstanceMatrix = this->NewAnimationInstance.GetNodeTransform(StartNode,this->AnimationDelta); 
        this->AnimationDelta += 0.016f;
    }
    catch (const std::runtime_error& e) {SysPrint(e.what(),3);}
}