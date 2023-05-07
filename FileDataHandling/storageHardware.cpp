#include "storageHardware.h"
#include <iostream>
#include <fstream>
#include <string>
using json = nlohmann::json;

// TODO: Update whole structure of StorageHardware

LabSH::storageHardware::storageHardware(storageHardwareConst const &sysConst)
    : SysConst(sysConst), block(sysConst.BLOCKCOUNT, std::string()), inodePList(sysConst.InodeCount, nullptr), FSMD(sysConst)
{
    for (auto &inodePtr : inodePList)
    {
        inodePtr = new LabSH::inode();
    }
    writeDnode(0, LabSH::dnode(SysConst.DirectorySize));
}

LabSH::storageHardware::~storageHardware()
{
    for (auto &inodePtr : inodePList)
    {
        delete inodePtr;
    }
}

bool LabSH::storageHardware::writeBlock(int ID, std::string data)
{
    if (data.length() <= SysConst.BLOCKSIZE)
    {
        block.at(ID) = data;
        return true;
    }
    return false;
}

std::string LabSH::storageHardware::readBlock(int ID) const
{
    return block.at(ID);
}

LabSH::inode LabSH::storageHardware::readInode(int ID)
{
    return *(inodePList.at(ID));
}

bool LabSH::storageHardware::writeFnode(int ID, LabSH::fnode fileNode)
{

    // delete previous object as it was on heap
    delete inodePList.at(ID);

    // Assign new object
    inodePList.at(ID) = new LabSH::fnode(fileNode);

    return true;
}

LabSH::fnode LabSH::storageHardware::readFnode(int ID)
{
    LabSH::inode *inodePtr = inodePList.at(ID);
    LabSH::fnode *fnodePtr = dynamic_cast<LabSH::fnode *>(inodePtr);
    // Return copy of this fnode
    return *fnodePtr;
}

bool LabSH::storageHardware::writeDnode(int ID, LabSH::dnode dirNode)
{
    // delete previous object as it was on heap
    delete inodePList.at(ID);

    // Assign new object
    inodePList.at(ID) = new LabSH::dnode(dirNode);

    return true;
}

LabSH::dnode LabSH::storageHardware::readDnode(int ID)
{
    LabSH::inode *inodePtr = inodePList.at(ID);
    LabSH::dnode *dnodePtr = dynamic_cast<LabSH::dnode *>(inodePtr);
    // Return copy of this fnode
    return *dnodePtr;
}

bool LabSH::storageHardware::deleteNode(int ID)
{
    // delete previous object as it was on heap
    delete inodePList.at(ID);

    // Assign new inode object so, that it's type appear as inode
    inodePList.at(ID) = new LabSH::inode();

    return true;
}

LabSH::FreeSpaceManagerData LabSH::storageHardware::readFSMD()
{
    return FSMD;
}

bool LabSH::storageHardware::writeFSMD(LabSH::FreeSpaceManagerData &fsmd)
{
    FSMD = fsmd;
    return true;
}

// json LabSH::StorageHardware::to_json(const storageHardware &storage)
// {
//     json j;
//     j["BLOCKSIZE"] = storage.SysConst.BLOCKSIZE;
//     j["BLOCKCOUNT"] = storage.SysConst.BLOCKCOUNT;
//     j["Block"] = storage.block;
//     return j;
// }

// LabSH::storageHardware LabSH::StorageHardware::from_json(const json &j)
// {

//     return storageHardware(j["BLOCKSIZE"].get<int>(), j["BLOCKCOUNT"].get<int>(), j["Block"].get<std::vector<std::string>>());
// }

// void LabSH::StorageHardware::write_to_file(storageHardware const &storage, std::string const &fname)
// {
//     json j;
//     j["Storage"] = to_json(storage);
//     std::ofstream file(fname);
//     file << j.dump(4); // 4 spaces for indentation
//     file.close();
// }

// LabSH::storageHardware LabSH::StorageHardware::build_from_file(std::ifstream &workFile)
// {
//     json j;
//     workFile >> j;

//     return from_json(j["Storage"]);
// }
