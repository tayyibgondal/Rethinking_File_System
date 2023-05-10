#include "../FileDataHandling/FileOperator.h"

/// @brief Lab: MultiUser File Management System
namespace LabFMS_MU
{
    struct OpenFileTableEntry
    {
        LabFO::FileOperator *const fileObj;
        std::string const User;

        int getFnodeID() const;
        bool isWriteMode() const;
        void deleteFileObj();
        bool isUserAllowed(std::string const &currentUser);

        OpenFileTableEntry(LabFO::FileOperator *const fileObj, std::string const &currentUser);
    };

    class OpenFileTable
    {
    private:
        std::list<OpenFileTableEntry> table;
        LabSM::FreeBlockManager &FBM;
        LabSH::storageHardware &SSD;
        std::string const &currentUser;

        OpenFileTableEntry *getFileEntre(int fnodeID);

        /// @return Pointer to table entry iff file is open in same mode else return *nullptr*
        OpenFileTableEntry *isOpenMode(int fnodeID, bool writeMode);

    public:
        OpenFileTable(LabSM::FreeBlockManager &FBM,
                      LabSH::storageHardware &SSD, std::string const &user);

        bool openFile(int fnodeID, bool writeMode);
        bool closeFile(int fnodeID);

        bool write_to_file(int fnodeID, std::string textData);
        bool write_to_file(int fnodeID, std::string textData, int startLocation);
        bool read_from_file(int fnodeID, std::string &output);
        bool read_from_file(int fnodeID, std::string &output, int start, int size);
        bool move_within_file(int fnodeID, int start, int size, int target);
        bool truncate_file(int fnodeID, int maxSize);
        bool clear_file(int fnodeID);
    };

    class FileManagementSystem
    {
    private:
        /* Simulating importing system package for storage interaction*/
        LabSH::storageHardware &SSD;

        LabSM::FreeSpaceManager FSM;
        OpenFileTable OFT;

        std::string CurrentUser;

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

        /// @param inode
        /// @param parentDnodeID Test for existance of Dnode at this ID before passing
        bool removeInodeEntry(int inode, int parentDnodeID);

        bool createObj(std::string objName, LabSH::nodeType objType);

        void Show_memory_map(std::stringstream &output, int PDNodeID = 0, bool recursive = true, std::string pad = ">");

    public:
        FileManagementSystem(LabSH::storageHardware &SSD);

        // For multiuser Capability
        void setUser(std::string NewUser);
        std::string const &getUserVarRef();

        bool createFile(std::string fname);
        bool deleteFile(std::string fname);

        bool createDir(std::string dirName);
        // bool changeDir(std::string dirName); Not Such thing

        bool moveFile(std::string source_fname, std::string target_fname);

        bool openFile(std::string fname, char mode);
        bool closeFile(std::string fname);

        bool write_to_file(std::string fname, std::string textData);
        bool write_to_file(std::string fname, std::string textData, int startLocation);
        std::string read_from_file(std::string fname);
        std::string read_from_file(std::string fname, int start, int size);
        bool move_within_file(std::string fname, int start, int size, int target);
        bool truncate_file(std::string fname, int maxSize);

        std::string Show_memory_map();
    };
}