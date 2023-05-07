#include "FreeSpaceManager.h"
#include <string>
#include <cmath>
// It includes "storageHardware.h"
namespace LabFO
{
    class FileOperator
    {
    private:
        LabSM::FreeBlockManager &BManager; // Block free space manager
        LabSH::storageHardware &SSD;
        LabSH::fnode fileNode;

        // FileNode
        int getFreeSpaceInLastBlock();
        int getCharBlockIndex(int charindex);

        // Test: File have enough logical capacity & SSD have enough free blocks
        // if yes return required new blocks count
        // Returns -1 if txtSize ==0
        int testGetNewBlockReq(int txtSize);
        bool isCharPositionExists(int i);

        // Pass them validated data
        bool write_to_file(std::string txt, int newBlockReq);
        std::string cut_from_file(int start, int size); // both parameters interms of char

    public:
        int const inode; // Node unique ID
        bool const writeMode;

        FileOperator(LabSM::FreeBlockManager &BManager,
                     LabSH::storageHardware &SSD,
                     int inode, bool writeMode = false);

        void saveFNode();
        bool write_to_file(std::string txt);
        bool write_to_file(int write_at, std::string txt);
        std::string read_from_file();
        std::string read_from_file(int start);
        std::string read_from_file(int start, int size); // both parameters interms of char
        bool move_within_file(int start, int size, int target);
        bool truncateFile(int maxSize);

        void clearFile();
    };
}