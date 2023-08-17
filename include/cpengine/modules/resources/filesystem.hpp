#pragma once

#include "../../definitions/typedefs.hpp"
#include <filesystem>
#include <vector>

namespace CPGFramework
{
    namespace Resources
    {
        typedef std::filesystem::path PATH;
        class FileSystem 
        {
        public:
            FileSystem();
            ~FileSystem();

            /// @brief Retrieves the path to the executable game folder.
            /// @return Returns the path to the executable game folder. 
            PATH& GetExecutableFolder();
            /// @brief Get the data folder path.
            /// @return Returns the formatted data folder path.
            STRING GetResourcesFolderPath();
            /// @brief Check if the file exists.
            /// @param relativePath The relative path, starting from the data folder (example: 'shaders' or 'models').
            ///                     Don't need to insert '/' at the begginning or at the end.
            /// @param fileName The file name with the extension to be checked.
            /// @return If the file exists.
            const BOOL FileExists(const STRING& relativePath, const STRING& filename);

            /// @brief Reads and store the file buffer, returning it.
            /// @param relativePath The relative path, starting from the data folder (example: 'shaders' or 'models').
            ///                     Don't need to insert '/' at the begginning or at the end.
            /// @param fileName The file name with extension.
            /// @return The file buffer.
            std::vector<char> ReadFileBuffer(const STRING& relativePath, const STRING& filename);
            /// @brief Helper function to split a string
            /// @param str 
            /// @param delimiter 
            /// @return 
            std::vector<STRING> SplitString(STRING str, const char delimiter);
        private:
            /// @brief Helper function to format the data path with the given relative path and filename.
            /// @param relativePath The relative path, starting from the data folder (example: 'shaders' or 'models').
            ///                     Don't need to insert '/' at the begginning or at the end.
            /// @param fileName The file name with extension.
            /// @return 
            STRING __INTERNAL__FormatDataPath(const STRING& relativePath, const STRING& filename);
        private:
            PATH m_execFolder;
        };
    } // namespace Resources
} // namespace CPGFramework