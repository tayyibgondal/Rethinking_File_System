#include <iostream>
#include <string>
#include <vector>
#include <math.h>
using namespace std;

class hehe
{
private:
    int n;

public:
    hehe(int n);
    ~hehe();
};

hehe::~hehe()
{
    cout << n << ": destructor\n";
}

void haha(hehe h)
{
    hehe *a = new hehe(h);
    int blblb;
}

struct storageHardwareConst
{
    int const BLOCKCOUNT,
        BLOCKSIZE, // interms of char
        InodeCount,
        FileMaxBlockCount, // interms of maxBlocks
        DirectorySize;     // interms of max name:inode pairs

        storageHardwareConst(int blockCount, int blockSize, int inodeCount, int FileMaxBlockCount, int directorySize)
        : BLOCKCOUNT(blockCount), FileMaxBlockCount(FileMaxBlockCount), DirectorySize(directorySize), BLOCKSIZE(BLOCKCOUNT + 99), InodeCount(inodeCount)
    {
    }
    int getFileMaxCharCount() const { return FileMaxBlockCount * BLOCKSIZE; }
};

int main(int argc, char const *argv[])
{
    storageHardwareConst a(1, 2, 3, 4, 5);
}
