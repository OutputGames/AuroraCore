#if !defined(SCENE_HPP)
#define SCENE_HPP

#include "entity.hpp"
#include "aurora/gfx/lighting.hpp"
#include "aurora/utils/utils.hpp"

struct SceneCreationSettings
{
    std::string Name;
    bool UseDefaultScene;
    bool SetAsCurrent;
};

struct AURORA_API Scene
{
    static Scene* Load(std::string filename, SceneCreationSettings settings);
    static Scene* Create(SceneCreationSettings settings);

    EntityMgr* entityMgr;
    LightingMgr* lightingMgr;

    void Update();
    void Serialize(std::string path);


    IS Scene* Current = nullptr;

    std::string Name;

private:
    Scene();
};


#endif // SCENE_HPP
