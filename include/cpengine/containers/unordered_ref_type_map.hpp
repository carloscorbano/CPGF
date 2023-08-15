#pragma once

#include "../definitions/dll.hpp"
#include <typeinfo>
#include <unordered_map>

namespace CPGFramework 
{
    namespace Containers 
    {
        typedef std::reference_wrapper<const type_info> RefTypeInfo;
        struct Hasher { size_t operator()(const RefTypeInfo& ref) const { return ref.get().hash_code(); } };
        struct EqualTo { bool operator()(const RefTypeInfo& lhs, const RefTypeInfo& rhs) const { return lhs.get().hash_code() == rhs.get().hash_code(); } };

        /// @brief A map that used the index as const type_info to link the typeid to a object T.
        /// @tparam T 
        template<typename T>
        struct UnorderedRefTypeMap : public std::unordered_map<RefTypeInfo, T, Hasher, EqualTo> {};
    }
}