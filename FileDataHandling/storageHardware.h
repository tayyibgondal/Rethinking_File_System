#pragma once
#include <vector>
#include <nlohmann/json.hpp>
// using json = nlohmann::json;

namespace LabSH
{
    typedef enum
    {
        LabInode,
        LabFile,
        LabDir,
        LabBlock
    } nodeType;

    struct storageHardwareConst
    {
        int const BLOCKCOUNT,
            BLOCKSIZE, // interms of char
            InodeCount,
            FileMaxBlockCount, // interms of maxBlocks
            DirectorySize;     // interms of max name:inode pairs
        storageHardwareConst(int blockCount, int blockSize, int inodeCount, int FileMaxBlockCount, int directorySize)
            : BLOCKCOUNT(blockCount), BLOCKSIZE(blockSize), InodeCount(inodeCount), FileMaxBlockCount(FileMaxBlockCount), DirectorySize(directorySize)
        {
        }
        int getFileMaxCharCount() const { return FileMaxBlockCount * BLOCKSIZE; }
    };

    struct FreeSpaceManagerData
    {
        std::vector<bool> blockStatus;
        std::vector<bool> inodeStatus;
        int freeBlockCount;
        int freeInodeCount;

        FreeSpaceManagerData(storageHardwareConst const &SysConst)
            : freeBlockCount(SysConst.BLOCKCOUNT), freeInodeCount(SysConst.InodeCount),
              blockStatus(SysConst.BLOCKCOUNT, false), inodeStatus(SysConst.InodeCount, false)
        {
            // Marking root node presence
            freeInodeCount--;
            inodeStatus.at(0) = true;
        }
        FreeSpaceManagerData(std::vector<bool> &blockStatus,
                             std::vector<bool> &inodeStatus,
                             int freeBlockCount,
                             int freeInodeCount)
            : freeBlockCount(freeBlockCount), freeInodeCount(freeInodeCount),
              blockStatus(blockStatus), inodeStatus(inodeStatus)
        {
        }
    };

    struct inode
    {
        nodeType const type;
        inode(nodeType type) : type(type) {}
        inode() : type(LabInode) {}
        virtual int getSize() { return 0; };
        virtual ~inode() {}
        // Ensure that the base class has a virtual destructor required for deleting derived class object through base class pointer
    };
    struct fnode : public inode
    {
        int blockCount; // Starts at 0
        int size;       // Number of characters
        std::vector<int> blockIdList;
        fnode(int maxBlockCount) // Default constructor
            : inode(LabFile), blockIdList(maxBlockCount, -1), size(0), blockCount(0)
        {
        }
        fnode(int maxBlockCount, int blockCount, int size, const std::vector<int> &array)
            : inode(LabFile), blockIdList(maxBlockCount, -1), size(0), blockCount(0) {}

        int getSize() override { return size; }
    };

    struct InodeEntry
    {
        std::string const name;
        int const inode; // inode ID

        InodeEntry(std::string const &name, int const &inode)
            : name(name), inode(inode) {}
    };

    struct dnode : public inode
    {
        int inodeCount; // initialised at 0
        std::vector<InodeEntry> inodeEntry;

        dnode(int DirectorySize)
            : inode(LabDir), inodeCount(0)
        {
        }
        dnode(int DirectorySize, const std::vector<InodeEntry> &inodeEntry)
            : inode(LabDir), inodeCount(0), inodeEntry(inodeEntry)
        {
        }

        int getSize() override { return getInodeCount(); }
        int getInodeCount() { return inodeCount; }
        bool addEntry(InodeEntry ie)
        {
            inodeEntry.push_back(ie);
            inodeCount++;
            return true;
        }
    };

    class storageHardware
    {
    private:
        std::vector<std::string> block;
        std::vector<inode *> inodePList;
        FreeSpaceManagerData FSMD;

        friend class StorageHardware;

    public:
        storageHardwareConst const SysConst;

        /// @brief This is for first boot-up
        /// @param sysConst
        storageHardware(storageHardwareConst const &sysConst);

        // TODO: Create for to be used to bootup from saved data
        // storageHardware(storageHardwareConst const &sysConst, const std::vector<std::string> &array);

        ~storageHardware();

        bool writeBlock(int ID, std::string data);
        std::string readBlock(int ID) const;

        LabSH::inode readInode(int ID);

        bool writeFnode(int ID, LabSH::fnode fileNode);
        LabSH::fnode readFnode(int ID);

        bool writeDnode(int ID, LabSH::dnode dirNode);
        LabSH::dnode readDnode(int ID);

        bool deleteNode(int ID);

        LabSH::FreeSpaceManagerData readFSMD();
        bool writeFSMD(LabSH::FreeSpaceManagerData &fsmd);
    };

    // class StorageHardware
    // {
    // private:
    //     static nlohmann::json to_json(const storageHardware &storage);
    //     // static storageHardware from_json(const nlohmann::json &j);

    // public:
    //     static void write_to_file(storageHardware const &storage, std::string const &fname);
    //     static storageHardware build_from_file(std::ifstream &workFile);
    // };
}