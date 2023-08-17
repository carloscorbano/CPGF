#include "resources.hpp"

namespace CPGFramework
{
    namespace Resources
    {
        ResourcesClass::ResourcesClass(Engine* engine)
            : IEngineModule(engine)
        {}

        ResourcesClass::~ResourcesClass()
        {}

        void ResourcesClass::Initialize()
        {
            _INTERNAL_Directory* rootDir = m_resTree.CreateData<_INTERNAL_Directory>(m_resTree.GetRoot());
            rootDir->name = "root";
        }

        void ResourcesClass::Cleanup()
        {
            Destroy();
        }

        FileSystem& ResourcesClass::GetFileSystem() 
        {
            return m_fs;
        }
        
        void ResourcesClass::Destroy() 
        {
            m_resTree.DestroyTree();
        }

        ResourcesClass::NODE ResourcesClass::__INTERNAL__GetDirectory(const STRING& path) 
        {
            std::vector<STRING> splittedPath = m_fs.SplitString(path, '/');

            NODE current = m_resTree.GetRoot();
            for(STRING dirName : splittedPath) 
            {
                BOOL found = false;
                m_resTree.ViewChildren<_INTERNAL_Directory>(current, [&](Containers::DataTree& tree, NODE& node, _INTERNAL_Directory& dir){
                    if(dir.name == dirName) 
                    {
                        found = true;
                        current = node;
                        return false;
                    }

                    return true;
                });

                if(!found) 
                {
                    current = __INTERNAL__CreateDirectoryNode(current, dirName);
                }
            }

            return current;
        }

        ResourcesClass::NODE ResourcesClass::__INTERNAL__GetFile(const NODE& directory, const STRING& filename) 
        {
            NODE result = m_resTree.GetRoot();
            BOOL success = false;
            m_resTree.ViewChildren<_INTERNAL_File>(directory, [&](Containers::DataTree& tree, NODE& node, _INTERNAL_File& file) 
            {
                if(file.filename == filename) 
                {
                    result = node;
                    success = true;
                    return false;
                }
                return true;
            });

            if(!success) 
            {
                result = __INTERNAL__CreateFileNode(directory, filename);
            }

            return result;
        }

        BOOL ResourcesClass::__INTERNAL__DirectoryExists(const STRING& path) 
        {
            std::vector<STRING> splittedPath = m_fs.SplitString(path, '/');

            NODE current = m_resTree.GetRoot();
            for(STRING dirPath : splittedPath) 
            {
                BOOL found = false;
                m_resTree.ViewChildren<_INTERNAL_Directory>(current, [&](Containers::DataTree& tree, NODE& node, _INTERNAL_Directory& dir)
                {
                    if(dir.name == dirPath) 
                    {
                        found = true;
                        current = node;
                        return false;
                    }

                    return true;
                });

                if(!found) 
                {
                    return false;
                }
            }

            return true;
        }

        BOOL ResourcesClass::__INTERNAL__FileExists(const STRING& path, const STRING& filename) 
        {
            if(!__INTERNAL__DirectoryExists(path)) 
            {
                return false;
            }

            NODE dirNode = __INTERNAL__GetDirectory(path);
            BOOL result = false;
            m_resTree.ViewChildren<_INTERNAL_File>(dirNode, [&](Containers::DataTree& tree, NODE& node, _INTERNAL_File& file) 
            {
                if(file.filename == filename) 
                {
                    result = true;
                    return false;
                }
                return true;
            });

            return result;
        }

        ResourcesClass::NODE ResourcesClass::__INTERNAL__CreateDirectoryNode(const NODE& owner, const STRING& dirName) 
        {
            NODE newDirNode = m_resTree.CreateNode();
            _INTERNAL_Directory* newDir = m_resTree.CreateData<_INTERNAL_Directory>(newDirNode);
            _INTERNAL_Directory* ownerDir = m_resTree.GetData<_INTERNAL_Directory>(owner);
            m_resTree.SetNodeOwner(newDirNode, owner);
            newDir->name = dirName;
            DEBUG_SUCCESS("Created new folder name: ", newDir->name, " - child of: ", ownerDir->name);
            return newDirNode;
        }

        ResourcesClass::NODE ResourcesClass::__INTERNAL__CreateFileNode(const NODE& directory, const STRING& filename) 
        {
            NODE fileNode = m_resTree.CreateNode();
            _INTERNAL_File* f = m_resTree.CreateData<_INTERNAL_File>(fileNode);
            f->filename = filename;
            m_resTree.SetNodeOwner(fileNode, directory);
            _INTERNAL_Directory* dirContent = m_resTree.GetData<_INTERNAL_Directory>(directory);
            DEBUG_SUCCESS("File not exists, creating file in folder: ", dirContent->name, " - name: ", filename);
            return fileNode;
        }

    } // namespace Resources
} // namespace CPGFramework