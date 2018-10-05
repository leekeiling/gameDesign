#include "agent.h"
#include "filter.h"
#include "scene.h"
#include "detour.h"
#include "Detour/Include/DetourNavMeshQuery.h"

#include "Detour/Include/DetourCommon.h"
#include <limits>

namespace NavMeshScene {

    const float ZERO[3] = { 0,0,0 };

    Agent::Agent()
        : mId(0)
        , mScene(nullptr)
        , mFilter(nullptr)
        , mCurPolyRef(0)
    {
        dtVcopy(mHalfExtents, DEFAULT_HALF_EXTENTS);
        dtVcopy(mPosition, ZERO);
        dtVcopy(mVelocity, ZERO);
    }

    Agent::~Agent() {

    }

    /*
        Need to overwrite the onHit function
        Update the agent's position. If the agent on target position
        will hit the other agents, it will stop anc handle this situation
        in onHit function.
        Final position will be stored in realEndPolyRef, and the data of scene need
        to be updated. 
    */
    void Agent::Update(float delta) {
        if (mVelocity[0] == 0 && mVelocity[1] == 0 && mVelocity[2] == 0) {
            return;
        }
        float endPos[3] = {
            mPosition[0] + mVelocity[0] * delta,
            mPosition[1] + mVelocity[1] * delta,
            mPosition[2] + mVelocity[2] * delta
        };
        // OnHit, set the agent's velocity zero
        // If the agent will hit other agents, it will stop moving.
        if (auto agent = checkPosByAOI(endPos[0], endPos[2]))
        {
            dtVcopy(mVelocity, ZERO);
            OnHit(agent);
            return;
        }
        bool bHit;
        uint64_t realEndPolyRef;
        float realEndPos[3];
        // final pos will stored in realEndPos
        if (!TryMove(endPos, realEndPolyRef, realEndPos, bHit)) {
            return;
        }
        if (bHit)
        {
            dtVcopy(mVelocity, ZERO);
            OnHit(nullptr);
            return;
        }
        mCurPolyRef = realEndPolyRef;
        dtVcopy(mPosition, realEndPos);
        //the agent move  out the current rectangle, need to update the scene.
        if (fabs(X - mPosition[0]) >= std::numeric_limits<float>::epsilon() || fabs(Y - mPosition[2]) >= std::numeric_limits<float>::epsilon())
        {
            X = mPosition[0];
            Y = mPosition[2];
            mScene->Update(this);
        }
    }

    /*
        Try to move to target position.
    */
    bool Agent::TryMove(float endPos[3], uint64_t& realEndPolyRef, float realEndPos[3], bool& bHit) {
        if (mScene)
        {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            return mScene->GetDetour().TryMove(
                mCurPolyRef,
                mPosition,
                endPos,
                mHalfExtents,
                filter.Get(),
                realEndPolyRef,
                realEndPos,
                bHit);
        }
        return false;
    }


    bool Agent::SetPosition(float v[3]) {
        if (mScene) {
            if (!checkPosByAOI(mPosition[0], mPosition[2]))
            {
                Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
                mScene->GetDetour().GetPoly(v, mHalfExtents, filter.Get(), mCurPolyRef, mPosition);
                X = mPosition[0];
                Y = mPosition[2];
                mScene->Update(this);  //need to insert on the scene
                return true;
            }
        }
        return false;
    }

    float randf()
    {
        return (float)(rand() / (float)RAND_MAX);
    }

    void Agent::RandomPosition() {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
        LABLE_RANDOM:
            mScene->GetDetour().RandomPosition(mHalfExtents, &filter.Get(), randf, mCurPolyRef, mPosition);
            if (checkPosByAOI(mPosition[0], mPosition[2]))
            {
                goto LABLE_RANDOM;
            }
            X = mPosition[0];
            Y = mPosition[2];
            mScene->Update(this);
        }
    }

    //raycast query 
    bool Agent::Raycast(float endPos[3], bool& bHit, float hitPos[3]) {
        if (mScene) {
            Filter& filter = mFilter ? *mFilter : mScene->GetDefaultFilter();
            return mScene->GetDetour().Raycast(
                mCurPolyRef,
                mPosition,
                endPos,
                filter.Get(),
                bHit,
                hitPos);
        }
        return false;
    }

    //This function is redundancy,
    //return the agent whose aoi retangle intersect current agent.
    Agent* Agent::checkPosByAOI(float& dstX, float& dstY)
    {
        aoi::Rect rect(dstX - mHalfExtents[0], dstX + mHalfExtents[0], dstY - mHalfExtents[2], dstY + mHalfExtents[2]);
        auto agents = mScene->Query(this, rect); 
        //get the agents within the rectangle
        for (Agent* a = (Agent*)agents; a; a = (Agent*)a->Next())
        {
            if (a == this)
            {
                continue;
            }
            aoi::Rect tempRect(a->getRect());
            if (rect.Intersects(tempRect))
            {
                return a;
            }
        }
        return nullptr;
    }
}
