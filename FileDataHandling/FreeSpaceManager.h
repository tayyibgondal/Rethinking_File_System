#include "storageHardware.h"

namespace LabSM
{
    class FreeBlockManager
    {
    public:
        virtual int getFreeBlock() = 0;
        virtual int getFreeBlockCount() = 0;
    };
    class FreeNodeManager
    {
    public:
        virtual int setFreeFnode() = 0;
        virtual int setFreeDnode() = 0;
    };

    class FreeSpaceManager : public FreeBlockManager, public FreeNodeManager
    {
        /*
        Status:
        - False: Not in used
        - True:  In use
        */
    private:
        LabSH::storageHardware const &SDD;
        std::vector<bool> blockStatus;
        std::vector<bool> fnodeStatus;
        std::vector<bool> dnodeStatus;
        int freeBlockCount;
        int freeFnodeCount;
        int freeDnodeCount;

        int findFirstFreeElement(std::vector<bool> *elementList); // Returns index of it
        int getFirstFreeElement(LabSH::nodeType list);            // Returns index of it mark it as In use

    public:
        FreeSpaceManager(LabSH::storageHardware &SDDptr);
        int getFreeBlock();
        int getFreeBlockCount();
        int getFreeFnode();
        int getFreeDnode();
        bool setFreeBlock(int ID);
        bool setFreeFnode(int ID);
        bool setFreeDnode(int ID);
    };
}