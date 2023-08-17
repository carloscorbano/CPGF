#pragma once

#include "../../definitions/dll.hpp"
#include "../../definitions/typedefs.hpp"
#include "../iengine_module.hpp"

#include <chrono>
#define TIMEPOINTS 2
#define TIMEPOINT_BEGIN 0
#define TIMEPOINT_END 1

namespace CPGFramework
{
    class Engine;
    namespace Time
    {
        typedef std::chrono::high_resolution_clock HRCLOCK;
        typedef HRCLOCK::time_point HRCLOCK_TIMEPOINT;
        
        class GameTime : public IEngineModule
        {
        public:
            friend class CPGFramework::Engine;
            GameTime(Engine* engine);
            ~GameTime();

        private:
            void Update() override;            

        public:
            /// @brief Retrieve the delta time, it's scaled by the delta scale.
            /// @return The delta time.
            const DOUBLE GetDeltaTime() const;
            /// @brief Retrieve the delta time, it's not affected by the delta scale.
            /// @return the unscaled delta time.
            const DOUBLE GetDeltaTimeUnscaled() const;
            /// @brief Set the delta time scale.
            void SetDeltaTimeScale(DOUBLE newScale = 1.0);
        private:
            /// @brief Get the time now.
            /// @return The timepoint.
            HRCLOCK_TIMEPOINT __INTERNAL__GetTimepointNow() const;
            /// @brief Calculate the elapsed time between two points.
            /// @param start The start point
            /// @param end The end point
            /// @return Return the elapsed time in milliseconds.
            DOUBLE __INTERNAL__GetElapsedTimeMS(const HRCLOCK_TIMEPOINT& start, const HRCLOCK_TIMEPOINT& end) const;
        private:
            HRCLOCK_TIMEPOINT m_points[TIMEPOINTS];
            DOUBLE m_delta;
            DOUBLE m_deltaScale;
        };
    } // namespace Time
} // namespace CPGFramework
