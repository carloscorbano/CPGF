#pragma once

namespace CPGFramework
{
    class Engine;
    class IEngineModule
    {
    public:
        inline IEngineModule(Engine* engine) : m_engine(engine) {}
        virtual ~IEngineModule() {}
        
        friend class Engine;
    protected:
        inline Engine& GetEngineRef() { return *m_engine; }

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void LateUpdate() = 0;
        virtual void Cleanup() = 0;
    private:
        Engine* m_engine;
    };
} // namespace CPGFramework
