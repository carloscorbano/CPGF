#include "filesystem.hpp"

#include <fstream>
#include <sstream>
#include <exception>

namespace CPGFramework
{
    namespace Resources
    {
        FileSystem::FileSystem() : m_execFolder(std::filesystem::current_path()) {}

        FileSystem::~FileSystem() {}

        PATH& FileSystem::GetExecutableFolder() 
        {
            return m_execFolder;
        }

        STRING FileSystem::GetResourcesFolderPath() 
        {
            return m_execFolder.generic_string().append("/resources");
        }

        const BOOL FileSystem::FileExists(const STRING& relativePath, const STRING& filename) 
        {
            return std::filesystem::exists(__INTERNAL__FormatDataPath(relativePath, filename));
        }

        std::vector<char> FileSystem::ReadFileBuffer(const STRING& relativePath, const STRING& filename) 
        {
            try 
            {
                STRING finalPath = __INTERNAL__FormatDataPath(relativePath, filename);
                std::fstream stream;
                stream.exceptions(std::ios::badbit | std::ios::failbit | std::ios::eofbit);
                stream.open(finalPath.c_str(), std::ios::in | std::ios::binary);

                if(stream.is_open()) 
                {
                    std::vector<char> buffer;

                    stream.seekg(0, stream.end);
                    size_t size = stream.tellg();
                    stream.seekg(0, stream.beg);

                    buffer.reserve(size);

                    char c;
                    for(size_t i = 0; i < size; ++i) 
                    {
                        stream.read(&c, 1);
                        buffer.push_back(c);
                    }
                    return buffer;
                }
            } 
            catch(const std::exception& ex) 
            {
                throw std::runtime_error(ex.what());
            }

            return std::vector<char>();
        }

        std::vector<STRING> FileSystem::SplitString(STRING str, const char delimiter) 
        {
            std::vector<STRING> result = std::vector<STRING>();

            if(str.empty()) 
            {
                return result;
            }

            std::stringstream stream;
            for(char c : str) 
            {
                if(c == delimiter) 
                {
                    if(!stream.str().empty()) 
                    {
                        result.push_back(stream.str());
                        stream.str(STRING());
                        stream.clear();
                    }
                } 
                else 
                {
                    stream << c;
                }
            }

            if(!stream.str().empty()) 
            {
                result.push_back(stream.str());
            }

            return result;
        }

        STRING FileSystem::__INTERNAL__FormatDataPath(const STRING& relativePath, const STRING& filename) 
        {
            if(relativePath.empty()) 
            {
                return GetResourcesFolderPath().append("/").append(filename);
            } 
            else 
            {
                return GetResourcesFolderPath().append("/").append(relativePath).append("/").append(filename);
            }
        }
    } // namespace Resources
} // namespace CPGFramework
