#pragma once

#include "../../definitions/typedefs.hpp"
#define INVALID_ID -1

namespace CPGFramework
{
    namespace Resources
    {
        enum class ResourceState 
        {
            NONE, CREATING, LOADING, ERROR, OK
        };

        struct ResourceBase 
        {
            ResourceBase(const STRING& relativePath, const STRING& filename)
                : m_relativePath(relativePath), m_filename(filename), alreadyCompiled(false) {}

            virtual ~ResourceBase() {}

            virtual void HotReload() {}

            STRING GetRelativePath() const 
            {
                return m_relativePath;
            }

            STRING GetFilename() const 
            {
                return m_filename;
            }

            const ResourceState GetResourceState() const 
            {
                return m_state;
            }

            void SetResourceState(const ResourceState& newState) 
            {
                m_state = newState;
            }
        protected:
            BOOL alreadyCompiled;
        private:
            STRING m_relativePath;
            STRING m_filename;
            ResourceState m_state;
        };
    } // namespace Resources
} // namespace CPGFramework
