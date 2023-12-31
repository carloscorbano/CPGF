#pragma once

#include "../iengine_module.hpp"
#include "../../events/event_emitter.hpp"

#include <memory>
#include <functional>

#define DEFAULT_WINDOW_TITLE "CPGF VERSION [0.0.1]"
#define DEFAULT_WINDOW_WIDTH 1320
#define DEFAULT_WINDOW_HEIGHT 780

class GLFWwindow;
struct GLFWmonitor;
namespace CPGFramework
{
    namespace Graphics
    {
        struct OnWindowCloseEvent {};

        class Window : public IEngineModule, public Events::EventEmitter
        {
        public:
            Window(Engine* engine);
            ~Window();
            friend class CPGFramework::Engine;

            inline GLFWwindow* GetMainContext() { return m_winCtx; }
            inline GLFWwindow* GetResourcesContext() { return m_resCtx; }
            inline const bool IsFPSLocked() const { return m_fpsLocked; }

            inline void SetFPSLocked(const bool locked) { m_fpsLocked = locked; }
        private:

            void SetThreadContext(GLFWwindow* ctx);
            bool WindowWaitEvents();

            void PollEvents();
            void Draw();
            
            virtual void Initialize() override;
            virtual void Cleanup() override;

            void __INTERNAL__centralizeWindow(GLFWmonitor* monitor);
        private:
            GLFWwindow* m_winCtx;
            GLFWwindow* m_resCtx;
            bool m_fpsLocked;
        };
    } // namespace Graphics
} // namespace CPGFramework