#include "FileSystemPrompt.h"
#include <iostream>
#include <fstream>

#include <thread>

using namespace std;

std::vector<std::string> *getFileNames(int threadCount);
std::vector<std::string> *getThreadFileNames(int threadCount, std::string prefix);

int main(int argc, char const *argv[])
{

    // check enough arguments
    if (argc < 2)
        return -1;

    // Note: First arg is exe name, second is file count, rest are files name
    int threadCount = stoi(argv[1]);

    // Files containing input strings
    std::vector<std::string> *inputFiles = getFileNames(threadCount);

    // Check: Files exists
    if (inputFiles == nullptr)
        return -1;

    std::vector<std::string> *outputFiles = getThreadFileNames(threadCount, "output");

    // System Setup
    LabSH::storageHardwareConst sysConst(100, 10, 20, 10, 10);
    LabSH::storageHardware SSD(sysConst);
    LabFMS_MU::FileManagementSystem FMS(SSD);
    LabFSP::FileSystemPrompt_MU FSP_MU(FMS);

    // mainRun
    bool continueRun = true;
    sem_t isFull;
    LabFSP::FSP_Threaded multiPrompt(FSP_MU, continueRun, isFull);

    // Create a vector of Prompt objects
    std::vector<LabFSP::Prompt> prompts;
    int ID = 0;

    for (int ID = 0; ID < threadCount; ID++)
    {
        prompts.push_back(LabFSP::Prompt(multiPrompt, std::to_string(ID), inputFiles->at(ID), outputFiles->at(ID)));
    }

    // Create threads to run Prompt objects
    std::vector<std::thread> threads;
    for (auto &prompt : prompts)
    {
        threads.push_back(std::thread(&LabFSP::Prompt::run, &prompt));
    }

    // Create thread to run MultiPrompt
    std::thread multiPromptThread(&LabFSP::FSP_Threaded::processJobList, &multiPrompt);

    // Join threads
    for (auto &t : threads)
    {
        t.join();
    }

    // All processing done
    continueRun = false;
    sem_post(&isFull);
    // Join MultiPrompt thread
    multiPromptThread.join();

    return 0;
}

std::vector<std::string> *getFileNames(int threadCount)
{
    // check enough arguments
    if (threadCount <= 0)
        return nullptr;

    std::vector<std::string> *inputFiles = getThreadFileNames(threadCount, "input");
    for (int count = 0; count < threadCount; count++)
    {
        std::string fileName = inputFiles->at(count);
        ifstream file(fileName);
        if (!file.is_open())
        {
            cout << "Error: " << fileName << " does not exist" << endl;
            return nullptr;
        }
        file.close();
    }

    return inputFiles;
}

std::vector<std::string> *getThreadFileNames(int threadCount, std::string const prefix)
{
    // check enough arguments
    if (threadCount <= 0)
        return nullptr;

    std::vector<std::string> *dataFiles = new std::vector<std::string>;
    for (int count = 0; count < threadCount; count++)
    {
        std::string fileName = prefix + "_thread<" + std::to_string(count) + ">.txt";
        dataFiles->emplace_back(fileName);
    }

    return dataFiles;
}
