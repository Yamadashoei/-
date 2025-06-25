#pragma once

class IScene {
public:
    virtual ~IScene() {}
    virtual void Initialize() = 0;
    virtual void Update(char* keys, char* preKeys) = 0;
    virtual void Draw() = 0;
    virtual bool IsEnd() const = 0;
    virtual int NextScene() const = 0;
};
