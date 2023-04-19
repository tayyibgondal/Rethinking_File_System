#pragma once
#include <vector>
#include <nlohmann/json.hpp>
// using json = nlohmann::json;

namespace LabSH
{
    typedef enum
    {
        LabFILE,
        LabDir,
        LabBlock
    } nodeType;

    struct storageHardwareConst
    {
        int const BLOCKCOUNT,
            BLOCKSIZE, // interms of char
            FnodeCount,
            DnodeCount,
            FileMaxBlockCount, // interms of maxBlocks
            DirectorySize;     // interms of max name:inode pairs
        storageHardwareConst(int blockCount, int blockSize, int fnodeCount, int dnodeCount, int maxBlockCount, int directorySize)
            : BLOCKCOUNT(blockCount), BLOCKSIZE(blockSize), FnodeCount(fnodeCount), DnodeCount(dnodeCount), FileMaxBlockCount(maxBlockCount), DirectorySize(directorySize)
        {
        }
        int getFileMaxCharCount() const { return FileMaxBlockCount * BLOCKSIZE; }
    };

    struct inode
    {
        nodeType type;
        inode(nodeType type) : type(type) {}
    };
    struct fnode : public inode
    {
        fnode(int maxBlockCount) // Default constructor
            : inode(LabFILE), blockIdList(maxBlockCount, NULL), size(0), blockCount(0)
        {
        }
        fnode(int maxBlockCount, int blockCount, int size, const std::vector<int> &array)
            : inode(LabFILE), blockIdList(maxBlockCount, NULL), size(0), blockCount(0) {}
        int blockCount; // Starts at 0
        int size;       // Number of characters
        std::vector<int> blockIdList;
    };

    class storageHardware
    {
    private:
        std::vector<std::string> block;
        std::vector<fnode> fnodeList;

        friend class StorageHardware;

    public:
        storageHardwareConst const SysConst;
        storageHardware(storageHardwareConst const &sysConst);
        storageHardware(storageHardwareConst const &sysConst, const std::vector<std::string> &array);
        bool writeBlock(int ID, std::string data);
        std::string readBlock(int ID) const;
    };

    class StorageHardware
    {
    private:
        static nlohmann::json to_json(const storageHardware &storage);
        static storageHardware from_json(const nlohmann::json &j);

    public:
        static void write_to_file(storageHardware const &storage, std::string const &fname);
        static storageHardware build_from_file(std::ifstream &workFile);
    };
}