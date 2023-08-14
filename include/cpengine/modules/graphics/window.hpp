#pragma once

#include "../../definitions/dll.hpp"
#include "../iengine_module.hpp"
#include <memory>
#include <functional>

#define DEFAULT_WINDOW_TITLE "CPGF VERSION [0.0.1]"
#define DEFAULT_WINDOW_WIDTH 1320
#define DEFAULT_WINDOW_HEIGHT 780

class GLFWwindow;

namespace CPGFramework
{
    namespace Graphics
    {
        class Window : public IEngineModule
        {
        public:
            Window(Engine* engine);
            ~Window();
            friend class CPGFramework::Engine;

            inline GLFWwindow* GetMainContext() { return m_winCtx; }
            inline GLFWwindow* GetResourcesContext() { return m_resCtx; }

            void SetThreadContext(GLFWwindow* ctx);
            void RunWindowThreadWork();
        private:
            virtual void Initialize() override;
            virtual void Update() override;
            virtual void FixedUpdate() override;
            virtual void LateUpdate() override;
        private:
            GLFWwindow* m_winCtx;
            GLFWwindow* m_resCtx;

            std::function<void()> onWindowCloseEvent;
        };
    } // namespace Graphics
} // namespace CPGFramework