#include "FileOperator.h"

int LabFO::FileOperator::getFreeSpaceInLastBlock()
{
    return fileNode.blockCount * SSD.SysConst.BLOCKSIZE - fileNode.size;
}

int LabFO::FileOperator::getCharBlockIndex(int charindex)
{
    return floor((double)(charindex) / SSD.SysConst.BLOCKSIZE);
}

int LabFO::FileOperator::testGetNewBlockReq(int txtSize)
{
    // Check: txt is valid OR File has capacity
    if (fileNode.size + txtSize > SSD.SysConst.getFileMaxCharCount() || txtSize == 0)
        return -1;

    // Check: Enough free blocks available
    int newBlockReq = ceil((txtSize - getFreeSpaceInLastBlock()) / (double)SSD.SysConst.BLOCKSIZE);
    if (newBlockReq > BManager.getFreeBlockCount())
        return -1;

    return newBlockReq;
}

LabFO::FileOperator::FileOperator(LabSM::FreeBlockManager &BManager, LabSH::storageHardware &SSD, int inode, bool writeMode)
    : BManager(BManager), SSD(SSD), inode(inode), writeMode(writeMode), fileNode(SSD.readFnode(inode))
{
}

bool LabFO::FileOperator::write_to_file(std::string txt, int newBlockReq)
{
    // initialising required variables
    int charIndex = getFreeSpaceInLastBlock();
    int indexBL = fileNode.blockCount - 1; // Index for blockIdList in fileNode pointing to last block id

    // Updating fnode
    fileNode.blockCount += newBlockReq;
    fileNode.size += txt.size();

    // Filling last datablock if space available
    if (charIndex > 0)
    {
        std::string updatedBlockData = SSD.readBlock(fileNode.blockIdList.at(indexBL)) + txt.substr(0, charIndex);
        SSD.writeBlock(fileNode.blockIdList.at(indexBL), updatedBlockData);
    }

    // Storing all new blocks data
    indexBL++; // poiniting to next free space in blockIdList
    int newBlockID;
    for (; newBlockReq > 0; newBlockReq--, charIndex += SSD.SysConst.BLOCKSIZE, indexBL++)
    {
        // Get new block
        newBlockID = BManager.getFreeBlock();

        // Save data in it
        SSD.writeBlock(newBlockID, txt.substr(charIndex, SSD.SysConst.BLOCKSIZE));

        // Save Block ID in blockIdList
        fileNode.blockIdList.at(indexBL) = newBlockID;
    }

    saveFNode();
    return true;
}

std::string LabFO::FileOperator::cut_from_file(int start, int size)
{
    // Contains DataToCut + trailing data
    std::string tempData = read_from_file(start);

    // removing tempData from file
    truncateFile(start);

    // Writing trailing data back
    // Here size gives starting char index of trailing data
    write_to_file(tempData.substr(size));

    // Return data to Cut
    return tempData.substr(0, size);
}

bool LabFO::FileOperator::isCharPositionExists(int i)
{
    if (i >= 0 && i < fileNode.size)
        return true;
    return false;
}

void LabFO::FileOperator::saveFNode()
{
    SSD.writeFnode(inode, fileNode);
}

bool LabFO::FileOperator::write_to_file(std::string txt)
{
    int newBlockReq = testGetNewBlockReq(txt.size());
    if (newBlockReq == -1)
        return false;

    return write_to_file(txt, newBlockReq);
}

bool LabFO::FileOperator::write_to_file(int write_at, std::string txt)
{
    int newBlockReq = testGetNewBlockReq(txt.size());
    if (newBlockReq == -1)
        return false;

    // Check: write_at points to char postion not outside data
    if (!isCharPositionExists(write_at))
        return false;

    std::string dataToRewrite = read_from_file(write_at);
    truncateFile(write_at); // index start from 0
    txt.append(dataToRewrite);

    return write_to_file(txt);
}

std::string LabFO::FileOperator::read_from_file()
{
    // Using it will be inefficient
    // return read_from_file(0, fileNode.size);

    std::string data;
    for (int i = 0; i < fileNode.blockCount; i++)
    {
        data.append(SSD.readBlock(fileNode.blockIdList.at(i)));
    }

    return data;
}

std::string LabFO::FileOperator::read_from_file(int start)
{
    // read from start to end of file
    return read_from_file(start, fileNode.size);
}

std::string LabFO::FileOperator::read_from_file(int start, int size)
{
    int firstBlockIndex = getCharBlockIndex(start);
    int lastBlockIndex = getCharBlockIndex(start + size - 1); // -1 as starting character is included

    std::string data;
    // Check: Whther start position lies within data
    if (firstBlockIndex >= fileNode.blockCount || start >= fileNode.size)
        return data; // Empty String

    // Check: Reuested data available
    if (lastBlockIndex >= fileNode.blockCount)
        // Return max data possible
        lastBlockIndex = fileNode.blockCount - 1;
    /* size is not altered because it is handled by substr() function
    if size is more than actual character present it return all char present
     */

    for (int i = firstBlockIndex; i <= lastBlockIndex; i++)
    {
        data.append(SSD.readBlock(fileNode.blockIdList.at(i)));
    }

    return data.substr(start % SSD.SysConst.BLOCKSIZE, size);
}

bool LabFO::FileOperator::move_within_file(int start, int size, int target)
{

    // Checking validity of points
    // 1.Points within file
    if (!(isCharPositionExists(start) && isCharPositionExists(start + size - 1) && isCharPositionExists(target)))
        return false;
    // 2.Target Position should not be within data to be moved
    if (target >= start && target <= start + size - 1)
        return false;

    // Setting target to it's updated location
    // when target char position is after the text to be moved
    if (target > start)
        target -= size;

    // Cut DataToMove
    std::string DataToMove = cut_from_file(start, size);

    // Insert back data
    write_to_file(target, DataToMove);
    saveFNode();
    return true;
}

bool LabFO::FileOperator::truncateFile(int maxSize)
{

    if (fileNode.size <= maxSize)
        return false; // Mean no effect on size of file

    int reqLastBlockIndex = getCharBlockIndex(maxSize - 1); // -1 as starting character is included

    // Release blocks not required
    for (; reqLastBlockIndex + 1 < fileNode.blockCount; fileNode.blockCount--)
    {
        BManager.setFreeBlock(fileNode.blockIdList.at(fileNode.blockCount - 1));
    }

    // Updaing last block so that size = maxSize
    std::string oldData = SSD.readBlock(fileNode.blockIdList.at(reqLastBlockIndex));
    std::string truncatedData = oldData.substr(0, maxSize % SSD.SysConst.BLOCKSIZE);
    SSD.writeBlock(fileNode.blockIdList.at(reqLastBlockIndex), truncatedData);
    fileNode.size = maxSize;
    saveFNode();
    return true;
}

void LabFO::FileOperator::clearFile()
{
    for (const auto blockID : fileNode.blockIdList)
    {
        fileNode.blockCount--;
        BManager.setFreeBlock(blockID);
    }
    saveFNode();
}
