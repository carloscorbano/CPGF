#include "debug.hpp"

namespace CPGFramework
{
    namespace Debug
    {
        DebugClass* DebugClass::instance = nullptr;

        DebugClass::DebugClass() : m_logContainer(DEBUG_LOG_CONTAINER_SIZE) {}

        DebugClass::~DebugClass() {}

        DebugClass* DebugClass::GetInstance()
        {
            if(instance == nullptr)
            {
                instance = new DebugClass();
            }

            return instance;
        }

        STRING DebugClass::__INTERNAL__getFormattedTime() 
        {
            time_t t;
            struct tm* tt = nullptr;
            time(&t);
            tt = localtime(&t);

            std::stringstream ss;
            ss << "[" << std::setw(2) << std::setfill('0')  << tt->tm_hour
            << ":" << std::setw(2) << std::setfill('0') << tt->tm_min
            << ":" << std::setw(2) << std::setfill('0') << tt->tm_sec << "]";

            return ss.str();
        }

        void DebugClass::__INTERNAL__PrintToConsole(const DebugType& type, const STRING& message) 
        {
            switch (type)
            {
                case DebugType::LOG:
                    Console::tint_white();
                    break;
                case DebugType::WARNING:
                    Console::tint_yellow();
                    break;
                case DebugType::ERROR:
                    Console::tint_red();
                    break;
                case DebugType::SUCCESS:
                    Console::tint_green();
                    break;
                default:
                    Console::tint_white();
                    break;
            }

            printf("%s\n", message.c_str());
            Console::reset_color();
        }

        Containers::GenericCyclicList<DebugData>& DebugClass::GetDebugContainer() 
        {
            return m_logContainer;
        }
    } // namespace Debug
} // namespace CPGFramework