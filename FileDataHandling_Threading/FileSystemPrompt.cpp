#include "FileSystemPrompt.h"
#include <iostream>

void LabFSP::FileSystemPrompt::setUser(std::string user)
{
    FMS.setUser(user);
}

LabFSP::FileSystemPrompt::FileSystemPrompt(LabFMS_MU::FileManagementSystem &FMS)
    : FMS(FMS) {}

std::string LabFSP::FileSystemPrompt::executeCommand(const std::string &input)
{
    std::string command;
    std::string arg1, arg2, arg3, arg4;
    std::istringstream iss(input);
    iss >> command >> arg1 >> arg2 >> arg3 >> arg4;

    command;

    // Output Stream
    std::stringstream sout;

    if (command == "set_user")
        FMS.setUser(arg1);

    else if (command == "create")
        sout << "File " << arg1 << " creation: " << boolToString(FMS.createFile(arg1)) << std::endl;

    else if (command == "delete")
        sout << "File " << arg1 << " deletion: " << boolToString(FMS.deleteFile(arg1)) << std::endl;

    else if (command == "mkdir")
        sout << "Dir " << arg1 << " creation: " << boolToString(FMS.createDir(arg1)) << std::endl;

    else if (command == "move")
        sout << "Moved File " << arg1 << " to " << arg2 << " : " << boolToString(FMS.moveFile(arg1, arg2)) << std::endl;

    else if (command == "open")
    {
        if (arg2 == "r")
        {
            sout << "File " << arg1 << " open for read: " << boolToString(FMS.openFile(arg1, 'r')) << std::endl;
        }
        else if (arg2 == "w")
        {
            sout << "File " << arg1 << " open for write: " << boolToString(FMS.openFile(arg1, 'w')) << std::endl;
        }
        else
        {
            sout << "Invalid file mode: " << arg2 << std::endl;
        }
    }
    else if (command == "close")
        sout << input << ": " << boolToString(FMS.closeFile(arg1)) << std::endl;

    else if (command == "write_to_file")
    {
        if (arg3.empty())
        {
            sout << command << " " << arg1 << ": " << boolToString(FMS.write_to_file(arg1, arg2)) << std::endl;
        }
        else
        {
            int start = stoi(arg3);
            sout << command << " " << arg1 << ": " << boolToString(FMS.write_to_file(arg1, arg2, start)) << std::endl;
        }
    }
    else if (command == "read_from_file")
    {
        if (arg2.empty())

            sout << command << " " << arg1 << ": " << FMS.read_from_file(arg1) << std::endl;
        else
        {
            int start = stoi(arg2);
            int size = stoi(arg3);
            sout << command << " " << arg1 << ": " << FMS.read_from_file(arg1, start, size) << std::endl;
        }
    }
    else if (command == "truncate_file")
    {
        int maxSize = stoi(arg2);
        sout << command << " " << arg1 << ": " << boolToString(FMS.truncate_file(arg1, maxSize)) << std::endl;
    }
    else if (command == "show_mem_map")
        sout << "Memory Map:\n"
             << FMS.Show_memory_map();

    else
        sout << "Invalid command: " << command << std::endl;

    return sout.str();
}

std::string LabFSP::FileSystemPrompt::boolToString(bool cond)
{
    return cond ? "successful" : "unsuccessful";
}

void LabFSP::FileSystemPrompt_MU::setUser(std::string user)
{
    if (currentUserRef != user)
        FSP.setUser(user);
}

LabFSP::FileSystemPrompt_MU::FileSystemPrompt_MU(LabFMS_MU::FileManagementSystem &FMS)
    : FSP(FMS), currentUserRef(FMS.getUserVarRef())
{
}

std::string LabFSP::FileSystemPrompt_MU::executeCommand(const std::string &input)
{
    int commandStart = input.find(" ");

    std::string const user = input.substr(0, commandStart);
    std::string const command = input.substr(commandStart);

    // Update: CurrentUser if request is of different user;
    this->setUser(user);

    return FSP.executeCommand(command);
}

LabFSP::FSP_CMD::FSP_CMD(LabFSP::FileSystemPromptInterface &FSP)
    : FSP(FSP)
{
}

void LabFSP::FSP_CMD::run()

{
    std::cout << "Welcome to the File System prompt!" << std::endl;
    std::cout << "Type 'help' for a list of available commands." << std::endl;

    std::string input;
    while (true)
    {
        std::cout << "> ";
        getline(std::cin, input);

        if (input == "help")
        {
            this->printHelp();
        }
        else if (input == "quit")
        {
            // To DO: Save state
            break;
        }
        else
        {
            std::cout << FSP.executeCommand(input);
        }
    }

    std::cout << "Goodbye!" << std::endl;
}

void LabFSP::FSP_CMD::printHelp()
{
    std::cout << "available commands:" << std::endl;
    std::cout << "  create <fname>" << std::endl;
    std::cout << "  delete <fname>" << std::endl;
    std::cout << "  mkdir <dirName>" << std::endl;
    std::cout << "  move <source_fname> <target_fname>" << std::endl;
    std::cout << "  open <fName> <mode:r/w>" << std::endl;
    std::cout << "  close <fName>" << std::endl;
    std::cout << "  write_to_file <filename> <text>" << std::endl;
    std::cout << "  write_to_file <filename> <text> <startLocation>" << std::endl;
    std::cout << "  read_from_file <filename> <start> <size>" << std::endl;
    std::cout << "  truncate_file <filename> <maxSize>" << std::endl;
    std::cout << "  show_mem_map" << std::endl;
    std::cout << "  Help" << std::endl;
    std::cout << "  Quit" << std::endl;
}