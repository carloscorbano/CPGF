#pragma once

#include "../../containers/data_tree.hpp"
#include "../iengine_module.hpp"
#include "resource_base.hpp"
#include "filesystem.hpp"
#include <mutex>
#include "../../debug/debug.hpp"

#define IS_RESOURCE_TYPE_DIRECTORY_POSITION 0

namespace CPGFramework
{
    namespace Resources
    {
        class ResourcesClass : public IEngineModule
        {
            struct _INTERNAL_Directory : public Containers::NodeData 
            {
                STRING name;
            };

            struct _INTERNAL_File : public Containers::NodeData 
            {
                STRING filename;
                ResourceBase* resource;
                i32 usage_count;
            };
        public:
            typedef Containers::DataTree::Node NODE;

            ResourcesClass(Engine* engine);
            ~ResourcesClass();

            void Initialize() override;
            void Cleanup() override;

            /// @brief Get the file system obj.
            /// @return 
            FileSystem& GetFileSystem();

            /// @brief Load or retrieve an existing file from the resource tree.
            /// @tparam T The type of the object.
            /// @tparam ...TArgs 
            /// @param relativePath The relative path from the resources folder.
            /// @param filename The filename
            /// @param ...args construction arguments (the relativePath and filename will be passed as well to the object creation.)
            /// @return The created or the loaded file.
            template<typename T, typename... TArgs>
            T* Load(const STRING& relativePath, const STRING& filename, TArgs... args) 
            {
                DEBUG_LOG("Load resource requested, path: ", relativePath, " - filename: ", filename);
                NODE directory = __INTERNAL__GetDirectory(relativePath);
                NODE fileNode = __INTERNAL__GetFile(directory, filename);
                return __INTERNAL__GetOrCreateFileData<T>(fileNode, relativePath, filename, args...);
            }

            /// @brief Retrieves a loaded resource from the tree. (It'll not create a new one).
            /// @tparam T The type of the resource file
            /// @param relativePath The relative path from the resources folder.
            /// @param filename The name of the file.
            /// @return The resource object or nullptr if it doesnt' exists.
            template<typename T>
            T* Get(const STRING& relativePath, const STRING& filename) 
            {
                if( !__INTERNAL__DirectoryExists(relativePath) ||
                    !__INTERNAL__FileExists(relativePath, filename)) 
                {
                        return nullptr;
                }

                NODE directory = __INTERNAL__GetDirectory(relativePath);
                NODE fileNode = __INTERNAL__GetFile(directory, filename);
                return __INTERNAL__GetOrCreateFileData<T>(fileNode);
            }

            /// @brief Unload the resource. If the usage count reaches zero, it'll be destroyed.
            template<typename T>
            void Unload(const STRING& relativePath, const STRING& filename) 
            {
                if( !__INTERNAL__DirectoryExists(relativePath) || 
                    !__INTERNAL__FileExists(relativePath, filename)) 
                {
                    return;
                }

                NODE directory = __INTERNAL__GetDirectory(relativePath);
                NODE fileNode = __INTERNAL__GetFile(directory, filename);

                _INTERNAL_File* fileContent = m_resTree.GetData<_INTERNAL_File>(fileNode);
                i32& usage_count = --fileContent->usage_count;
                DEBUG_LOG("Unloading resource file, path: ", relativePath, " name: ", filename, " usage count: ", std::to_string(usage_count));
                if(usage_count <= 0) 
                {
                    DEBUG_WARNING("Resource usage count reached zero, deleting resource.");

                    if(fileContent->resource) 
                    {
                        delete fileContent->resource;
                        fileContent->resource = nullptr;
                    }

                    m_resTree.FreeNode(fileNode);

                    if(m_resTree.GetNodeChildrenCount(directory) == 0) 
                    {
                        _INTERNAL_Directory* dirContent = m_resTree.GetData<_INTERNAL_Directory>(directory);
                        DEBUG_WARNING("The number of elements in the folder reached zero, starting to clean. Directory name: ", dirContent->name);
                        m_resTree.FreeNode(directory);
                    }
                }
            }

            /// @brief Destroy the resources container.
            void Destroy();
        private:
            NODE __INTERNAL__GetDirectory(const STRING& path);
            NODE __INTERNAL__GetFile(const NODE& directory, const STRING& filename);
            
            BOOL __INTERNAL__DirectoryExists(const STRING& path);
            BOOL __INTERNAL__FileExists(const STRING& path, const STRING& filename);

            NODE __INTERNAL__CreateDirectoryNode(const NODE& owner, const STRING& dirName);
            NODE __INTERNAL__CreateFileNode(const NODE& directory, const STRING& filename);

            template<typename T, typename... TArgs>
            T* __INTERNAL__GetOrCreateFileData(NODE& fileNode, TArgs... args) 
            {
                _INTERNAL_File* fileContent = m_resTree.GetData<_INTERNAL_File>(fileNode);

                if(fileContent->resource) 
                {
                    ResourceBase* rb = fileContent->resource;
                    if(rb->GetResourceState() == ResourceState::ERROR) 
                    {
                        rb->HotReload();
                    }

                    fileContent->usage_count++;
                    DEBUG_SUCCESS("Resource successfully retrieved from database, name ", fileContent->filename, " - usage count: ", std::to_string(fileContent->usage_count));
                } 
                else 
                {
                    fileContent->resource = new T(&GetEngineRef(), args...);
                    fileContent->usage_count = 1;
                    DEBUG_SUCCESS("Created resource file name ", fileContent->filename, ", starting to run data creation pipeline.");
                }

                return reinterpret_cast<T*>(fileContent->resource);
            }
        private:
            Containers::DataTree m_resTree;
            FileSystem m_fs;
            std::mutex m_mutex;
        };
    } // namespace Resources
} // namespace CPGFramework
