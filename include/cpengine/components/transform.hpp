#pragma once

#include "../definitions/typedefs.hpp"
#include "../containers/data_tree.hpp"
namespace CPGFramework
{
    namespace World { class WorldClass; }
    namespace Components
    {
        class Transform
        {
        public:
            friend class World::WorldClass;
            Transform();
            ~Transform();

            void SetParent(Transform& parent);

            const MAT4 GetLocalMatrix() const { return m_local; }
        private:
            Containers::DataTree::Node worldNode;
            Containers::DataTree::Node parentNode;
            
            MAT4 m_local;
        };
    } // namespace Components
} // namespace CPGFramework