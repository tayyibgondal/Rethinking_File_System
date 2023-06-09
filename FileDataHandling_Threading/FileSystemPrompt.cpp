#include <iostream>
#include <fstream>
#include <chrono>
#include "FileSystemPrompt.h"

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

LabFSP::FSP_Threaded::FSP_Threaded(LabFSP::FileSystemPrompt_MU &FSP, bool const &continueRun, sem_t &isFull)
    : FSP(FSP), continueRun(continueRun), isFull(isFull)
{
    sem_init(&isFull, 0, 0);
    sem_init(&jsem, 0, 1);

    // Create EmptyFile
    std::ofstream logfile("log.txt");
    if (logfile.is_open())
    {
        logfile.close();
    }
}

void LabFSP::FSP_Threaded::processJobList()

{
    std::cout << "Started Working\n";
    while (true)
    {
        sem_wait(&isFull);
        // Check whether last sem_post() was a termination call;
        if (!continueRun)
            break;
        sem_wait(&jsem);

        std::string input = jobs.front();
        jobs.pop_front();

        // Update output for client
        std::string output = FSP.executeCommand(input);
        *output_pointers.front() = output;
        output_pointers.pop_front();

        sem_post(semaphores.front());
        semaphores.pop_front();
        sem_post(&jsem);

        // Log to file
        time_t now = time(nullptr);
        char timestamp[30];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
        long int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() % 1000;
        char ms[4];
        sprintf(ms, "%03ld", milliseconds);
        std::string log_entry = std::string(timestamp) + "." + std::string(ms) + " " + input + " " + output + "\n";
        std::ofstream logfile("log.txt", std::ios::app);
        if (logfile.is_open())
        {
            logfile << log_entry;
            logfile.close();
        }
    }
    std::cout << "Stopped Working\n";
}

void LabFSP::FSP_Threaded::addJob(const std::string &job, std::string &output, sem_t *sema)
{
    sem_wait(&jsem);
    jobs.push_back(job);
    semaphores.push_back(sema);
    output_pointers.push_back(&output);
    sem_post(&isFull);
    sem_post(&jsem);
}

bool LabFSP::Prompt::logOutput(std::string const &output)
{
    std::ofstream logfile(outputFile, std::ios::app);
    if (logfile.is_open())
    {
        logfile << output;
        logfile.close();
        return true;
    }
    return false;
}

LabFSP::Prompt::Prompt(LabFSP::FSP_Threaded &multiPrompt, const std::string userID, const std::string &inputFile, std::string const &outputFile)
    : multiPrompt(multiPrompt), inputFile(inputFile), userID(userID), outputFile(outputFile)
{
    sem_init(&sem, 0, 0);

    // Create EmptyFile
    std::ofstream logfile(outputFile);
    if (logfile.is_open())
    {
        logfile.close();
    }
}

void LabFSP::Prompt::run()
{
    std::ifstream input(this->inputFile);

    if (input.is_open())
    {
        std::string line;
        std::string output;
        while (std::getline(input, line))
        {
            line = userID + " " + line;
            multiPrompt.addJob(line, output, &sem);
            sem_wait(&sem);
            // Now it knows ans is here
            this->logOutput(output);
        }
        input.close();
    }
    else
    {
        std::cout << "Unable to open file: " << this->inputFile << std::endl;
    }
}
