#include "storageHardware.h"
#include <iostream>
#include <fstream>
#include <string>
using json = nlohmann::json;

// TODO: Update whole structure of StorageHardware

LabSH::storageHardware::storageHardware(storageHardwareConst const &sysConst)
    : SysConst(sysConst)
{
}

LabSH::storageHardware::storageHardware(storageHardwareConst const &sysConst, const std::vector<std::string> &array)
    : SysConst(sysConst), block(array)
{
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

json LabSH::StorageHardware::to_json(const storageHardware &storage)
{
    json j;
    j["BLOCKSIZE"] = storage.SysConst.BLOCKSIZE;
    j["BLOCKCOUNT"] = storage.SysConst.BLOCKCOUNT;
    j["Block"] = storage.block;
    return j;
}

// LabSH::storageHardware LabSH::StorageHardware::from_json(const json &j)
// {

//     return storageHardware(j["BLOCKSIZE"].get<int>(), j["BLOCKCOUNT"].get<int>(), j["Block"].get<std::vector<std::string>>());
// }

void LabSH::StorageHardware::write_to_file(storageHardware const &storage, std::string const &fname)
{
    json j;
    j["Storage"] = to_json(storage);
    std::ofstream file(fname);
    file << j.dump(4); // 4 spaces for indentation
    file.close();
}

LabSH::storageHardware LabSH::StorageHardware::build_from_file(std::ifstream &workFile)
{
    json j;
    workFile >> j;

    return from_json(j["Storage"]);
}
