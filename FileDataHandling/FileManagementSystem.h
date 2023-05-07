#include "FileOperator.h"
#include <list>

namespace LabFMS
{
    struct OpenFileTableEntry
    {
        LabFO::FileOperator *const fileObj;
        int getFnodeID() const;
        bool isWriteMode() const;
        void deleteFileObj();

        OpenFileTableEntry(LabFO::FileOperator *const fileObj);
    };

    class OpenFileTable
    {
    private:
        std::list<OpenFileTableEntry> table;
        LabSM::FreeBlockManager &FBM;
        LabSH::storageHardware &SSD;

        OpenFileTableEntry *getFileEntre(int fnodeID);

        /// @return Pointer to table entry iff file is open in same mode else return *nullptr*
        OpenFileTableEntry *isOpenMode(int fnodeID, bool writeMode);

    public:
        OpenFileTable(LabSM::FreeBlockManager &FBM,
                      LabSH::storageHardware &SSD);

        bool openFile(int fnodeID, bool writeMode);
        bool closeFile(int fnodeID);

        bool write_to_file(int fnodeID, std::string textData);
        bool write_to_file(int fnodeID, std::string textData, int startLocation);
        bool read_from_file(int fnodeID, std::string &output);
        bool read_from_file(int fnodeID, std::string &output, int start, int size);
        bool move_within_file(int fnodeID, int start, int size, int target);
        bool truncate_file(int fnodeID, int maxSize);
    };

    class FileManagementSystem
    {
    private:
        /* Simulating importing system package for storage interaction*/
        LabSH::storageHardware &SSD;

        LabSM::FreeSpaceManager FSM;
        OpenFileTable OFT;

        /// @param path Validated Path Format: "/Dir1/Dir12/filename.ext"
        /// @return String Vector ["Dir1","Dir12","filename.ext"]
        std::vector<std::string> parsePath(std::string path);

        ///@brief  Always start at root "/" dnode
        ///@param path Full path starting from root.
        ///@return
        /// If parent Directory exists returns it's inode and update path to object name
        /// else return -1
        int getParentDirNodeID(std::string &path);

        /// @param name FileName or DirName
        /// @param parentDnodeID Test for existance of Dnode at this ID before passing
        /// @return inode of entry with name "name" if it such entry exists else -1
        int getInodeID(std::string const &name, int parentDnodeID);

        int getFileIndoeID(std::string filePath);

        bool createObj(std::string objName, LabSH::nodeType objType);

    public:
        FileManagementSystem(LabSH::storageHardware &SSD);

        bool createFile(std::string fname);
        bool deleteFile(std::string fname);

        bool createDir(std::string dirName);
        // bool changeDir(std::string dirName); Not Such thing
        // [ ] bool moveFile(std::string source_fname, std::string target_fname);

        bool openFile(std::string fname, char mode);
        bool closeFile(std::string fname);

        bool write_to_file(std::string fname, std::string textData);
        bool write_to_file(std::string fname, std::string textData, int startLocation);
        std::string read_from_file(std::string fname);
        std::string read_from_file(std::string fname, int start, int size);
        bool move_within_file(std::string fname, int start, int size, int target);
        bool truncate_file(std::string fname, int maxSize);

        //[ ] bool Show_memory_map();
    };
}