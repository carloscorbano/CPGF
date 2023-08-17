#include "game_time.hpp"

namespace CPGFramework
{
    namespace Time
    {
        GameTime::GameTime(Engine* engine)
            : IEngineModule(engine) 
        {
            m_points[TIMEPOINT_BEGIN] = m_points[TIMEPOINT_END] = __INTERNAL__GetTimepointNow();
            m_delta = 0.0;
            m_deltaScale = 1.0;
        }

        GameTime::~GameTime() {}

        void GameTime::Update() 
        {
            m_points[TIMEPOINT_END] = __INTERNAL__GetTimepointNow();
            m_delta = __INTERNAL__GetElapsedTimeMS(m_points[TIMEPOINT_BEGIN], m_points[TIMEPOINT_END]);
            m_points[TIMEPOINT_BEGIN] = m_points[TIMEPOINT_END];
        }

        const DOUBLE GameTime::GetDeltaTime() const 
        {
            return m_delta * m_deltaScale;
        }

        const DOUBLE GameTime::GetDeltaTimeUnscaled() const 
        {
            return m_delta;
        }

        void GameTime::SetDeltaTimeScale(DOUBLE newScale) 
        {
            m_deltaScale = newScale;
        }

        HRCLOCK_TIMEPOINT GameTime::__INTERNAL__GetTimepointNow() const 
        {
            return HRCLOCK::now();
        }

        DOUBLE GameTime::__INTERNAL__GetElapsedTimeMS(const HRCLOCK_TIMEPOINT& start, const HRCLOCK_TIMEPOINT& end) const 
        {
            return (std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()) * 1e-9;
        }
    } // namespace Time
} // namespace CPGFramework