#pragma once

#include "../definitions/typedefs.hpp"
#include "../definitions/dll.hpp"
#include <vector>
#include <stack>
#include <functional>
#include <map>
#include <mutex>

#define INVALID_ID -1

namespace CPGFramework 
{
    namespace Containers 
    {
        struct NodeData 
        {
            NodeData() {};
            virtual ~NodeData() {};
        };

        class CPGF_API DataTree 
        {
            /// @brief Internal node information.
            struct _INTERNAL_NODE
            {
                const i32 index;
                i32 owner;
                std::vector<i32> children;
                
                NodeData* container;
                _INTERNAL_NODE(const i32& index) : index(index), owner(INVALID_ID), children(std::vector<i32>()), container(nullptr) {}
            };
        public:
            /// @brief Struct that the user uses to iteract with this class.
            struct Node 
            {
                friend class DataTree;
                Node& operator=(const Node& other) 
                {
                    if(&other != this) index = other.index;
                    return *this;
                }

                private:
                    Node(const i32& index) : index(index) {}
                    i32 index;
            };

            DataTree() 
            {
                __INTERNAL__CreateNode(); /*create root node.*/ 
            }

            ~DataTree() 
            {
                __INTERNAL__DestroyTree();
            }
            
            /// @brief Creates a new node of this tree.
            /// @return 
            Node CreateNode() 
            {
                return Node(__INTERNAL__GetAvailableNode()); 
            }

            /// @brief Set the node owner.
            /// @param node 
            /// @param owner 
            void SetNodeOwner(const Node& node, const Node& owner) 
            {
                __INTERNAL__SetOwner(node.index, owner.index);
            }

            /// @brief Get the owner node of the given node.
            /// @param node 
            /// @return 
            Node GetNodeOwner(const Node& node) 
            {
                return Node(node.index);
            }

            /// @brief Get the number of children nodes of the given node.
            /// @param node 
            /// @return 
            i32 GetNodeChildrenCount(const Node& node) const 
            {
                return (i32)m_nodes[node.index].children.size();
            }
            
            /// @brief Release the node.
            /// @param node 
            void FreeNode(const Node& node) 
            {
                __INTERNAL__FreeNode(node.index);
            }

            /// @brief Retrieve the root node.
            /// @return 
            Node GetRoot() 
            {
                return Node(0);
            }

            /// @brief Create a data inside of the given node. If the node already contain a created data, then it'll be deleted.
            /// @tparam T The data type.
            /// @tparam ...Args params
            /// @param node The owner node.
            /// @param ...args Construction arguments.
            /// @return A pointer to the created data.
            template<typename T, typename... Args>
            DLL_EXPORT T* CreateData(const Node& node, Args... args) 
            {
                __INTERNAL__CleanNodeData(node.index);
                return __INTERNAL__CreateData<T>(node.index, args...);
            }

            /// @brief Retrieves the data from the given node.
            /// @tparam T 
            /// @param node 
            /// @return 
            template<typename T>
            DLL_EXPORT T* GetData(const Node& node) 
            {
                return __INTERNAL__GetData<T>(node.index);
            }

            /// @brief Iterates within the tree, retrieving only the types of T.
            /// @tparam T The type that will be retrieve from the tree
            /// @param operation The operation that will run for each object of type T.
            /// @param topToBottom If the search will be from top to bottom or from bottom to top.
            template<typename T>
            DLL_EXPORT void View(std::function<void(DataTree& tree, Node& node, T& data, BOOL& cancellationToken)> operation, BOOL topToBottom = true) 
            {
                View(Node(0), operation, topToBottom);
            }

            /// @brief Iterates within the tree, retrieving only the types of T and starting from the given node.
            /// @tparam T The type that will be retrieve from the tree
            /// @param operation The operation that will run for each object of type T.
            /// @param topToBottom If the search will be from top to bottom or from bottom to top.
            template<typename T>
            DLL_EXPORT void View(const Node& starterNode, std::function<void(DataTree& tree, Node& node, T& data, BOOL& cancellationToken)> operation, BOOL topToBottom = true) 
            {
                BOOL cancellationToken = false;
                __INTERNAL__View(starterNode.index, operation, topToBottom, cancellationToken);
            }

            /// @brief Iterates only with the children of the given node.
            /// @tparam T The type of data to be iterated over.
            /// @param owner The node that will have it's children iterated.
            /// @param operation The operation that will be triggered to process the match data. If the operation returns false, then it'll stop the iteration.
            template<typename T>
            DLL_EXPORT void ViewChildren(const Node& owner, std::function<BOOL(DataTree& tree, Node& node, T& data)> operation) 
            {
                i32 ownerID = owner.index;
                for(i32& child : m_nodes[ownerID].children) 
                {
                    T* d = __INTERNAL__GetData<T>(child);
                    if(d) 
                    {
                        if(!operation(*this, Node(child), *d)) 
                        {
                            break;
                        }
                    }
                }
            }

            /// @brief Destroys the tree.
            void DestroyTree() 
            {
                __INTERNAL__DestroyTree();
            }
        private:
            /// @brief Helper method to create a new node.
            /// @return The node index.
            i32 __INTERNAL__CreateNode() 
            {
                i32 index = m_nodes.size();
                m_nodes.push_back(_INTERNAL_NODE(index));
                return index;
            }

            /// @brief Helper method to retrieve an available node (use this instead of Create node).
            /// @return The index of the node.
            i32 __INTERNAL__GetAvailableNode() 
            {
                i32 index;
                if(m_freeIndexes.empty()) 
                {
                    index = __INTERNAL__CreateNode();
                } 
                else 
                {
                    index = m_freeIndexes.top();
                    m_freeIndexes.pop();
                }

                return index;
            }

            /// @brief Helper method to create the inner data of the node.
            /// @tparam T 
            /// @tparam ...Args 
            /// @param index the index of the ndoe.
            /// @param ...args construction arguments.
            /// @return The ptr to the created data.
            template<typename T, typename... Args>
            T* __INTERNAL__CreateData(const i32& index, Args... args) 
            {
                m_nodes[index].container = new T(args...);
                return reinterpret_cast<T*>(m_nodes[index].container);
            }

            /// @brief Helper method to retrieve the data from the given node.
            /// @tparam T 
            /// @param node 
            /// @return 
            template<typename T>
            T* __INTERNAL__GetData(const i32& index) 
            {
                if(!m_nodes[index].container) 
                {
                    return nullptr;
                }

                return reinterpret_cast<T*>(m_nodes[index].container);
            }

            /// @brief Helper method to clean the node data.
            /// @param index The index of the node.
            void __INTERNAL__CleanNodeData(const i32& index) 
            {
                if(m_nodes[index].container) 
                {
                    delete m_nodes[index].container;
                    m_nodes[index].container = nullptr;
                }
            }
            
            /// @brief Helper method to free the node.
            /// @param index The index of the node that will be freed.
            void __INTERNAL__FreeNode(const i32& index) 
            {
                std::vector<i32>& children = m_nodes[index].children;
                for(i32 child : children) 
                {
                    __INTERNAL__FreeNode(child);
                }

                __INTERNAL__RemoveFromOwner(index);
                __INTERNAL__CleanNodeData(index);
                m_freeIndexes.push(index);
            }

            /// @brief Helper method to set the node owner.
            /// @param index The index of the node.
            /// @param ownerIndex The owner index.
            void __INTERNAL__SetOwner(const i32& index, const i32& ownerIndex) 
            {
                _INTERNAL_NODE& cur = m_nodes[index];
                _INTERNAL_NODE& owner = m_nodes[ownerIndex];

                if(cur.owner != INVALID_ID) 
                {
                    __INTERNAL__RemoveFromOwner(cur.index);
                }

                cur.owner = owner.index;
                owner.children.push_back(cur.index);
            }

            /// @brief Helper method to remove the node from it's owner.
            /// @param index 
            void __INTERNAL__RemoveFromOwner(const i32& index) 
            {
                _INTERNAL_NODE& node = m_nodes[index];
                if(node.owner != INVALID_ID) 
                {
                    _INTERNAL_NODE& owner = m_nodes[node.owner];

                    std::vector<i32>::iterator it = std::find(owner.children.begin(), owner.children.end(), node.index);

                    if(it != owner.children.end()) 
                    {
                        owner.children.erase(it);
                    }

                    node.owner = INVALID_ID;
                }
            }

            /// @brief Helper method to Destroy this tree data.
            void __INTERNAL__DestroyTree() 
            {
                for(i32 i = 0; i < m_nodes.size(); ++i) 
                {
                    __INTERNAL__CleanNodeData(i);
                }

                m_nodes.clear();
            }

            /// @brief Helper method to iterates within the tree, searching for object with the same type T.
            /// @tparam T 
            /// @param currentIndex 
            /// @param operation 
            /// @param topToBottom 
            /// @param cancellationToken 
            template<typename T>
            void __INTERNAL__View(i32 currentIndex, std::function<void(DataTree& tree, Node& node, T& data, BOOL& cancellationToken)> operation, BOOL topToBottom, BOOL& cancellationToken) 
            {
                _INTERNAL_NODE& curNode = m_nodes[currentIndex];
                std::vector<i32>& children = curNode.children;

                if(topToBottom) 
                {
                    T* d = GetData<T>(Node(currentIndex));
                    if(d && operation) 
                    {
                        operation(*this, Node(currentIndex), *d, cancellationToken);
                    }
                }

                for(i32 child : children) 
                {
                    if(cancellationToken) 
                    {
                        break;
                    }
                    __INTERNAL__View(child, operation, topToBottom, cancellationToken);
                }

                if(!topToBottom) 
                {
                    T* d = GetData<T>(Node(currentIndex));
                    if(d && operation) 
                    {
                        operation(*this, Node(currentIndex), *d, cancellationToken);
                    }
                }
            }
        private:
            std::vector<_INTERNAL_NODE> m_nodes;
            std::stack<i32> m_freeIndexes;
        };
    }
} // namespace CeosFramework