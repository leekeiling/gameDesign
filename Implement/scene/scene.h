#ifndef __NMS_SCENE_H__
#define __NMS_SCENE_H__

#include <memory>
#include <unordered_map>
#include "aoi/aoi.h"


namespace NavMeshScene {
    class Agent;
}

typedef aoi::Scene<NavMeshScene::Agent, 16> AOISceneType;

namespace NavMeshScene {

    class Detour;
    class Filter;

    class Scene : public AOISceneType
    {
    public:

        Scene(bool bStatic);
        virtual ~Scene();

        int Load(const char* path); //load the map, .bin
        void AddAgent(uint64_t id, const std::shared_ptr<Agent>& agent); //
        void RemoveAgent(uint64_t id);

        float* GetBoundsMin();
        float* GetBoundsMax();

    public:
        inline Detour& GetDetour() { return *mDetour; }
        inline Filter& GetDefaultFilter() { return *mDefaultFilter; }

    protected:
        std::unique_ptr<Detour> mDetour; //
        std::unordered_map<uint64_t, std::shared_ptr<Agent>> mAgents;
        std::unique_ptr<Filter> mDefaultFilter;
    };

    //static scene
    class StaticScene : public Scene {
    public:
        StaticScene() : Scene(true) {}
        virtual ~StaticScene() {}
    };

    //to do for dynamic scene
}

#endif
