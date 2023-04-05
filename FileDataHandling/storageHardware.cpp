#include<string>

class storageHardware
{
private:
    std::string * block;    
    int const BLOCKSIZE;
public:
    storageHardware(int blockSize,int blockCount);
    ~storageHardware();
};

storageHardware::storageHardware(int blockSize,int blockCount)
{
    this->BLOCKSIZE = blockSize;
    block = new string[blockCount];
}

storageHardware::~storageHardware()
{
    delete[] block;
}
