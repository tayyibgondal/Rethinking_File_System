#include "FileManagementSystem.h"

std::vector<std::string> LabFMS::FileManagementSystem::parsePath(std::string path)
{
    // Remove first '/'
    path.erase(0, 1);

    // Create a stringstream from the path string
    std::stringstream ss(path);

    // Create a vector to store the split strings
    std::vector<std::string> result;

    // Split the path string using '/' as the delimiter
    while (ss.good())
    {
        std::string substr;
        getline(ss, substr, '/');
        result.push_back(substr);
    }

    return result;
}

int LabFMS::FileManagementSystem::getParentDirNodeID(std::string &path)
{
    int parentDnodeID = 0;
    std::vector<std::string> parsedPath = parsePath(path);
    int const lastIndex = parsedPath.size() - 1;

    for (int index = 0; index < lastIndex; index++) // as last element is the object itself
    {
        parentDnodeID = getInodeID(parsedPath.at(index), parentDnodeID);

        // Check: Existance of parent
        if (parentDnodeID == -1)
            return -1;

        // Check: Parent must be directory
        if (!(SSD.readInode(parentDnodeID).type == LabSH::LabDir))
            return -1;
    }

    // Update path to actual object name
    path = parsedPath.at(lastIndex);
    return parentDnodeID;
}

int LabFMS::FileManagementSystem::getFileIndoeID(std::string filePath)
{
    // if it exists then dirName will be updated by the function "getParentDirNodeID" to file name
    int ParentDirNodeID = getParentDirNodeID(filePath);

    // Check: Parent Directory Exist
    if (ParentDirNodeID == -1)
        return -1;

    int fnodeID = getInodeID(filePath, ParentDirNodeID);

    // Check: File Exists
    if (fnodeID == -1)
        return -1;

    // Check: obj must be file
    if (!(SSD.readInode(fnodeID).type == LabSH::LabFile))
        return -1;

    return fnodeID;
}

int LabFMS::FileManagementSystem::getInodeID(std::string const &name, int parentDnodeID)
{
    LabSH::dnode parentDnode = SSD.readDnode(parentDnodeID);
    for (const auto &inodeEntry : parentDnode.inodeEntry)
    {
        if (inodeEntry.name == name)
            return inodeEntry.inode;
    }

    return -1;
}

bool LabFMS::FileManagementSystem::createObj(std::string objName, LabSH::nodeType objType)
{
    // Check: Inode Available
    if (FSM.getFreeInodeCount() == 0)
        return false;

    // if it exists then dirName will be updated by the function "getParentDirNodeID" to new obj name
    int ParentDirNodeID = getParentDirNodeID(objName);

    // Check: Parent Directory Exist
    if (ParentDirNodeID == -1)
        return false;

    LabSH::dnode parentDir = SSD.readDnode(ParentDirNodeID);

    // Check: Parent Directory has free space
    if (parentDir.getInodeCount() >= SSD.SysConst.DirectorySize)
        return false;

    int newDnodeID = FSM.getFreeInode();

    // Write new obj on SSD
    switch (objType)
    {
    case LabSH::LabFile:
        SSD.writeFnode(newDnodeID, LabSH::fnode(SSD.SysConst.FileMaxBlockCount));
        break;
    case LabSH::LabDir:
        SSD.writeDnode(newDnodeID, LabSH::dnode(SSD.SysConst.DirectorySize));
        break;
    }

    parentDir.addEntry(LabSH::InodeEntry(objName, newDnodeID));

    // Write back updated Parent DNode
    SSD.writeDnode(ParentDirNodeID, parentDir);

    return true;
}

LabFMS::FileManagementSystem::FileManagementSystem(LabSH::storageHardware &SSD)
    : SSD(SSD), FSM(SSD), OFT(FSM, SSD)
{
}

bool LabFMS::FileManagementSystem::createFile(std::string fname)
{
    return createObj(fname, LabSH::LabFile);
}

bool LabFMS::FileManagementSystem::deleteFile(std::string fname)
{
    int fnodeID = getFileIndoeID(fname);

    // Check: File Exists
    if (fnodeID == -1)
        return false;

    // Release blocks held by file
    LabFO::FileOperator fileObj(FSM, SSD, fnodeID, true);
    fileObj.clearFile();
    fileObj.saveFNode();

    // Delete fnode: make inode free
    SSD.deleteNode(fnodeID);

    return true;
}

bool LabFMS::FileManagementSystem::createDir(std::string dirName)
{
    return createObj(dirName, LabSH::LabDir);
}

bool LabFMS::FileManagementSystem::openFile(std::string fname, char mode)
{
    bool writeMode;
    switch (mode)
    {
    case 'w':
        writeMode = true;
        break;
    case 'r':
        writeMode = false;
        break;

    default:
        // invalid mode
        return false;
    }
    return OFT.openFile(getFileIndoeID(fname), writeMode);
}

bool LabFMS::FileManagementSystem::closeFile(std::string fname)
{
    return OFT.closeFile(getFileIndoeID(fname));
}

bool LabFMS::FileManagementSystem::write_to_file(std::string fname, std::string textData)
{
    return OFT.write_to_file(getFileIndoeID(fname), textData);
}

bool LabFMS::FileManagementSystem::write_to_file(std::string fname, std::string textData, int startLocation)
{
    return OFT.write_to_file(getFileIndoeID(fname), textData, startLocation);
}

std::string LabFMS::FileManagementSystem::read_from_file(std::string fname)
{
    std::string output;
    OFT.read_from_file(getFileIndoeID(fname), output);
    return output;
}

std::string LabFMS::FileManagementSystem::read_from_file(std::string fname, int start, int size)
{
    std::string output;
    OFT.read_from_file(getFileIndoeID(fname), output, start, size);
    return output;
}

bool LabFMS::FileManagementSystem::move_within_file(std::string fname, int start, int size, int target)
{
    return OFT.move_within_file(getFileIndoeID(fname), start, size, target);
}

bool LabFMS::FileManagementSystem::truncate_file(std::string fname, int maxSize)
{
    return OFT.truncate_file(getFileIndoeID(fname), maxSize);
}

int LabFMS::OpenFileTableEntry::getFnodeID() const
{
    return fileObj->inode;
}

bool LabFMS::OpenFileTableEntry::isWriteMode() const
{
    return fileObj->writeMode;
}

void LabFMS::OpenFileTableEntry::deleteFileObj()
{
    delete fileObj;
}

LabFMS::OpenFileTableEntry::OpenFileTableEntry(LabFO::FileOperator *const fileObj)
    : fileObj(fileObj)
{
}

LabFMS::OpenFileTableEntry *LabFMS::OpenFileTable::getFileEntre(int fnodeID)
{
    for (auto &entre : table)
    {
        if (entre.getFnodeID() == fnodeID)
            return &entre;
    }

    return nullptr;
}

LabFMS::OpenFileTableEntry *LabFMS::OpenFileTable::isOpenMode(int fnodeID, bool writeMode)
{
    // Check: File open
    OpenFileTableEntry *entrePtr = getFileEntre(fnodeID);
    if (entrePtr == nullptr)
        return nullptr;

    // Check: Open mode is "writeMode"
    if (!((*entrePtr).isWriteMode() == writeMode))
        return nullptr; // File open in opposite mode

    return entrePtr;
}

LabFMS::OpenFileTable::OpenFileTable(LabSM::FreeBlockManager &FBM, LabSH::storageHardware &SSD)
    : FBM(FBM), SSD(SSD)
{
}

bool LabFMS::OpenFileTable::openFile(int fnodeID, bool writeMode)
{
    // Check: File Already open
    LabFMS::OpenFileTableEntry *entrePtr = getFileEntre(fnodeID);
    if (entrePtr != nullptr)
    {
        // Check: Open but in same mode
        if ((*entrePtr).isWriteMode() == writeMode)
            return true;
        else
            return false; // File open in opposite mode
    }

    // Open File and save it in table
    LabFMS::OpenFileTableEntry newEntre(new LabFO::FileOperator(FBM, SSD, fnodeID, writeMode));
    table.push_back(newEntre);

    return true;
}

bool LabFMS::OpenFileTable::closeFile(int fnodeID)
{
    int oldSize = table.size();
    table.remove_if([fnodeID](LabFMS::OpenFileTableEntry i)
                    { return i.getFnodeID() == fnodeID; });

    return oldSize != table.size();
}

bool LabFMS::OpenFileTable::write_to_file(int fnodeID, std::string textData)
{
    // Check: File Already open in write mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, true);
    if (entrePtr == nullptr)
        return false;

    return (*entrePtr).fileObj->write_to_file(textData);
}

bool LabFMS::OpenFileTable::write_to_file(int fnodeID, std::string textData, int startLocation)
{
    // Check: File Already open in write mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, true);
    if (entrePtr == nullptr)
        return false;

    return (*entrePtr).fileObj->write_to_file(startLocation, textData);
}

bool LabFMS::OpenFileTable::read_from_file(int fnodeID, std::string &output)
{
    // Check: File Already open in read mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, false);
    if (entrePtr == nullptr)
        return false;

    output = (*entrePtr).fileObj->read_from_file();
    return true;
}

bool LabFMS::OpenFileTable::read_from_file(int fnodeID, std::string &output, int start, int size)
{
    // Check: File Already open in read mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, false);
    if (entrePtr == nullptr)
        return false;

    output = (*entrePtr).fileObj->read_from_file(start, size);
    return true;
}

bool LabFMS::OpenFileTable::move_within_file(int fnodeID, int start, int size, int target)
{
    // Check: File Already open in write mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, true);
    if (entrePtr == nullptr)
        return false;

    return (*entrePtr).fileObj->move_within_file(start, size, target);
}

bool LabFMS::OpenFileTable::truncate_file(int fnodeID, int maxSize)
{
    // Check: File Already open in write mode
    LabFMS::OpenFileTableEntry *entrePtr = isOpenMode(fnodeID, true);
    if (entrePtr == nullptr)
        return false;

    return (*entrePtr).fileObj->truncateFile(maxSize);
}
