#pragma once

#include "../../definitions/dll.hpp"
#include "../../definitions/typedefs.hpp"

namespace CPGFramework
{
    namespace Time
    {
        struct TimedAction 
        {
            /// @brief Create the timed action with the given interval (in seconds)
            /// @param interval The interval that this action will return true in the Tick function (in seconds).
            TimedAction(const DOUBLE& interval) 
                : m_interval(interval), m_timer(0.0), m_fpsCount(0.0), m_fpsTimer(0.0), m_updateInterval(false), m_nextInterval(0.0) {}

            /// @brief Update the timer and returns if the interval was completed.
            /// @param unscaledDeltaTime The unscaled game delta time.
            /// @return True if the interval was completed.
            inline BOOL Tick(const DOUBLE& unscaledDeltaTime) 
            {
                m_timer += unscaledDeltaTime;
                m_fpsTimer += unscaledDeltaTime;

                if(m_fpsTimer > 1.0) 
                {
                    m_fps = m_fpsCount;
                    m_fpsTimer = 0.0;
                    m_fpsCount = 0.0;
                }
                
                if(m_timer > m_interval) 
                {
                    if(m_updateInterval) 
                    {
                        m_updateInterval = false;
                        m_interval = m_nextInterval;
                    }

                    DOUBLE lag = fmod(m_timer, m_interval);
                    m_timer = lag;
                    m_fpsCount++;
                    return true;
                }

                return false;
            }

            /// @brief Get the FPS count for this timer.
            /// @return The FPS count
            const DOUBLE GetFPS() const { return m_fps; }
            /// @brief Set the interval for this timer.
            /// @param newInterval The new interval in seconds.
            void SetInterval(const DOUBLE& newInterval) { m_nextInterval = newInterval; m_updateInterval = true; }
            /// @brief Retrieves the current interval of this timer.
            /// @return The interval in seconds.
            const DOUBLE GetInterval() const { return m_interval; }
        private:
            DOUBLE m_interval;
            DOUBLE m_timer;
            DOUBLE m_fpsCount;
            DOUBLE m_fps;
            DOUBLE m_fpsTimer;

            BOOL m_updateInterval;
            DOUBLE m_nextInterval;
        };
    }
}