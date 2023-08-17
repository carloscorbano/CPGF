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

        virtual void Initialize() {}
        virtual void Update() {}
        virtual void FixedUpdate() {}
        virtual void LateUpdate() {}
        virtual void Cleanup() {}
    private:
        Engine* m_engine;
    };
} // namespace CPGFramework
