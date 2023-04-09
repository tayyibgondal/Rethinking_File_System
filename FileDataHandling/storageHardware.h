#pragma once
#include <vector>
#include <nlohmann/json.hpp>
// using json = nlohmann::json;

namespace LabSH
{
    class storageHardware
    {
    private:
        std::vector<std::string> block;
        int const BLOCKSIZE, BLOCKCOUNT;

        friend class StorageHardware;

    public:
        storageHardware(int blockSize, int blockCount);
        storageHardware(int blockSize, int blockCount, const std::vector<std::string> &array);
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