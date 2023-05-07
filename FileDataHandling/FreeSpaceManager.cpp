#include "FreeSpaceManager.h"

int LabSM::FreeSpaceManager::findFirstFreeElement(std::vector<bool> *elementList)
{
    for (int i = 0, maxIndex = elementList->size(); i < maxIndex; i++)
    {
        if (elementList->at(i) == false)
            return i;
    }

    return -1;
}

int LabSM::FreeSpaceManager::getFirstFreeElement(LabSH::nodeType list)
{
    std::vector<bool> *elementList;
    int *freeElementCount;

    switch (list)
    {
    case LabSH::LabInode: // inode List
        elementList = &FSMD.inodeStatus;
        freeElementCount = &FSMD.freeInodeCount;
        break;

    case LabSH::LabBlock: // Block List
        elementList = &FSMD.blockStatus;
        freeElementCount = &FSMD.freeBlockCount;
        break;
    default:
        return -1;
    }

    int index = findFirstFreeElement(elementList);
    if (index != -1)
    {
        elementList->at(index) = true; // Mark it as in use
        (*freeElementCount)--;         // Decrement Free element available
    }
    return index;
}

LabSM::FreeSpaceManager::FreeSpaceManager(LabSH::storageHardware &SDDptr)
    : SSD(SDDptr), FSMD(SDDptr.readFSMD())

{
}

int LabSM::FreeSpaceManager::getFreeBlock()
{
    return getFirstFreeElement(LabSH::LabBlock);
}

int LabSM::FreeSpaceManager::getFreeBlockCount() const
{
    return FSMD.freeBlockCount;
}

int LabSM::FreeSpaceManager::getFreeInode()
{
    return getFirstFreeElement(LabSH::LabInode);
}

int LabSM::FreeSpaceManager::getFreeInodeCount() const
{
    return FSMD.freeInodeCount;
}

bool LabSM::FreeSpaceManager::setFreeBlock(int ID)
{
    if (ID < 0 || ID >= SSD.SysConst.BLOCKCOUNT) // Invalid Operation
        return false;

    FSMD.freeBlockCount++;
    FSMD.blockStatus.at(ID) = false;
    return true;
}
bool LabSM::FreeSpaceManager::setFreeInode(int ID)
{
    if (ID < 0 || ID >= SSD.SysConst.InodeCount) // Invalid Operation
        return false;

    FSMD.freeInodeCount++;
    FSMD.inodeStatus.at(ID) = false;
    return true;
}
