#ifndef __NMS_AI_H__
#define __NMS_AI_H__

#include <memory>
#include <cmath>
#include "aoi/aoi.h"
#include "filter.h"
#include "agent.h"

namespace NavMeshScene {
	class Filter;
	class Agent;
	class AI : public NavMeshScene::Agent
	{
	public:
		AI() 
		{
			std::shared_ptr<Filter> mFilter(new Filter());
			mFilter -> SetIncludeFlags(DEFAULT_INCLUDE_FLAGS);
			mFilter->SetExcludeFlags(DEFAULT_EXCLUDE_FLAGS);
			SetFilter(mFilter);
		}
		
		~AI()
		{
			
		}
	public:
	    void OnHit(Agent* agent) override
	    {
	        changeDir();
	    }
	
	public:
	    void changeDir()
	    {
	        float angle = float(rand() % 360);
	        float vx = cos(3.14f * angle / 180);
	        float vy = -sin(3.14f * angle / 180);
	        float s = sqrt(vx*vx + vy*vy);
	        vx = vx / s;
	        vy = vy / s;
	        float v[3] = { vx * 5, 0, vy * 5 };
	        SetVelocity(v);
	    }
	};
}
#endif
