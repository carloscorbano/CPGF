#pragma once

#include "entt.hpp"
#include "../../containers/data_tree.hpp"
namespace CPGFramework
{
    namespace World
    {
        struct EntityNode : public Containers::NodeData
        {
            entt::entity id;
        };
    } // namespace World
} // namespace CPGFramework
