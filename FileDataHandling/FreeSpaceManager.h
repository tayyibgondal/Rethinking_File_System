#include "storageHardware.h"

namespace LabSM
{
    class FreeBlockManager
    {
    public:
        virtual int getFreeBlock() = 0;
        virtual int getFreeBlockCount() const = 0;
        virtual bool setFreeBlock(int ID) = 0;
    };

    class FreeNodeManager
    {
    public:
        virtual int getFreeInodeCount() const = 0;
        virtual bool setFreeInode(int ID) = 0;
        virtual int getFreeInode() = 0;
    };

    class FreeSpaceManager : public FreeBlockManager, public FreeNodeManager
    {
        /*
        Status:
        - False: Not in used
        - True:  In use
        */
    private:
        LabSH::storageHardware const &SSD;
        LabSH::FreeSpaceManagerData FSMD;

        int findFirstFreeElement(std::vector<bool> *elementList); // Returns index of it
        int getFirstFreeElement(LabSH::nodeType list);            // Returns index of it mark it as In use

    public:
        FreeSpaceManager(LabSH::storageHardware &SDDptr);
        int getFreeBlock() override;
        int getFreeBlockCount() const override;
        int getFreeInode() override;
        int getFreeInodeCount() const override;
        bool setFreeBlock(int ID) override;
        bool setFreeInode(int ID) override;
    };
}