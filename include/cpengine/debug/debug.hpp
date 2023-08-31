#pragma once

#include "console.hpp"
#include "../containers/generic_cyclic_list.hpp"
#include "../definitions/typedefs.hpp"

#include <vector>
#include <numeric>
#include <ctime>
#include <mutex>
#include <sstream>
#include <iomanip>
#include <memory>

#define DEBUG_LOG_CONTAINER_SIZE 300


namespace CPGFramework
{
    class Engine;
    namespace Debug
    {
         enum class DebugType { LOG, WARNING, ERROR, SUCCESS };

        struct DebugData 
        {
            DebugType type;
            STRING message;
        };

        class DebugClass 
        {
            DebugClass();
        public:
            friend class CPGFramework::Engine;
            ~DebugClass();

            static DebugClass* GetInstance();
            
            /// @brief Submit the debug data into the container and print to the screen.
            template<typename... Args>
            void Submit(const DebugType& type, Args&&... args) 
            {
                std::lock_guard<std::mutex> lock(m);

                std::ostringstream o;
                __INTERNAL__ConcatArgs(o, __INTERNAL__getFormattedTime(), std::forward<Args>(args)...);
                __INTERNAL__PrintToConsole(type, o.str());
                m_logContainer.Insert({ type, o.str() });
            }

            /// @brief Get the log container.
            Containers::GenericCyclicList<DebugData>& GetDebugContainer();

        private:
            /// @brief Helper method to format the time (it'll print [HH::MM::SS]).
            /// @return The formatted time string.
            STRING __INTERNAL__getFormattedTime();
            /// @brief Helper method to print to console the data.
            /// @param data 
            void __INTERNAL__PrintToConsole(const DebugType& type, const STRING& message);

            template<typename T>
            void __INTERNAL__ConcatArgs(std::ostream& o, const T& t) 
            {
                o << t << " ";
            }

            template<typename T, typename... Args>
            void __INTERNAL__ConcatArgs(std::ostream& o, const T& t, Args&&... args) 
            {
                __INTERNAL__ConcatArgs(o, t);
                __INTERNAL__ConcatArgs(o, std::forward<Args>(args)...);
            }

        private:
            static DebugClass* instance;
            Containers::GenericCyclicList<DebugData> m_logContainer;
            std::mutex m;
        };
    } // namespace Debug
} // namespace CPGFramework

#define DEBUG(DebugType, ...) CPGFramework::Debug::DebugClass::GetInstance()->Submit(DebugType, __VA_ARGS__)
#define DEBUG_LOG(...) DEBUG(CPGFramework::Debug::DebugType::LOG, __VA_ARGS__)
#define DEBUG_WARNING(...) DEBUG(CPGFramework::Debug::DebugType::WARNING, __VA_ARGS__)
#define DEBUG_ERROR(...) DEBUG(CPGFramework::Debug::DebugType::ERROR, __VA_ARGS__)
#define DEBUG_SUCCESS(...) DEBUG(CPGFramework::Debug::DebugType::SUCCESS, __VA_ARGS__)