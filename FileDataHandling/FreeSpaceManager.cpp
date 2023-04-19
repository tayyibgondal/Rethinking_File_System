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

    switch (list)
    {
    case LabSH::LabFILE: // Fnode List
        elementList = &fnodeStatus;
        break;
    case LabSH::LabDir: // Dnode List
        elementList = &dnodeStatus;
        break;

    case LabSH::LabBlock: // Block List
        elementList = &blockStatus;
        break;
    default:
        return -1;
    }

    int index = findFirstFreeElement(elementList);
    if (index != -1)
        elementList->at(index) = true; // Mark it as in use

    return index;
}

LabSM::FreeSpaceManager::FreeSpaceManager(LabSH::storageHardware &SDDptr)
    : SDD(SDDptr),
      freeBlockCount(SDD.SysConst.BLOCKCOUNT), freeDnodeCount(SDD.SysConst.DnodeCount), freeFnodeCount(SDD.SysConst.FnodeCount),
      blockStatus(SDD.SysConst.BLOCKCOUNT, false), dnodeStatus(SDD.SysConst.DnodeCount, false), fnodeStatus(SDD.SysConst.FnodeCount, false)

{
}

int LabSM::FreeSpaceManager::getFreeBlock()
{
    return getFirstFreeElement(LabSH::LabBlock);
}

int LabSM::FreeSpaceManager::getFreeBlockCount()
{
    return freeBlockCount;
}

int LabSM::FreeSpaceManager::getFreeFnode()
{
    return getFirstFreeElement(LabSH::LabFILE);
}

int LabSM::FreeSpaceManager::getFreeDnode()
{
    return getFirstFreeElement(LabSH::LabDir);
}

bool LabSM::FreeSpaceManager::setFreeBlock(int ID)
{
    if (ID < 0 || ID >= SDD.SysConst.BLOCKCOUNT) // Invalid Operation
        return false;

    blockStatus.at(ID) = false;
    return true;
}
bool LabSM::FreeSpaceManager::setFreeFnode(int ID)
{
    if (ID < 0 || ID >= SDD.SysConst.FnodeCount) // Invalid Operation
        return false;

    fnodeStatus.at(ID) = false;
    return true;
}
bool LabSM::FreeSpaceManager::setFreeDnode(int ID)
{
    if (ID < 0 || ID >= SDD.SysConst.DnodeCount) // Invalid Operation
        return false;

    dnodeStatus.at(ID) = false;
    return true;
}
