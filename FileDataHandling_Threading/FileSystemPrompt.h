#include "FileManagementSystem.h"
#include <semaphore.h>
#include <deque>

/// @brief Lab: Fi;le System Prompt
namespace LabFSP
{

    class FileSystemPromptInterface
    {

    public:
        virtual std::string executeCommand(const std::string &input) = 0;
    };

    class FileSystemPrompt : public FileSystemPromptInterface
    {
    private:
        /// @return cond ? "successful" : "unsuccessful";
        std::string boolToString(bool cond);
        LabFMS_MU::FileManagementSystem &FMS;

    public:
        FileSystemPrompt(LabFMS_MU::FileManagementSystem &FMS);

        std::string executeCommand(const std::string &input);
        void setUser(std::string user);
    };

    class FileSystemPrompt_MU : public FileSystemPromptInterface
    {
    private:
        LabFSP::FileSystemPrompt FSP;
        std::string const &currentUserRef;

        void setUser(std::string user);

    public:
        FileSystemPrompt_MU(LabFMS_MU::FileManagementSystem &FMS);
        std::string executeCommand(const std::string &input);
    };

    class FSP_CMD
    {
    private:
        LabFSP::FileSystemPromptInterface &FSP;

    public:
        FSP_CMD(LabFSP::FileSystemPromptInterface &FSP);
        void run();
        void printHelp();
    };

    class FSP_Threaded
    {
    private:
        LabFSP::FileSystemPrompt_MU &FSP;
        sem_t &isFull;
        sem_t jsem;
        bool const &continueRun;
        std::deque<std::string> jobs;
        std::deque<sem_t *> semaphores;
        std::deque<std::string *> output_pointers;

    public:
        FSP_Threaded(LabFSP::FileSystemPrompt_MU &FSP, bool const &continueRun, sem_t &isFull);

        void processJobList();
        void addJob(const std::string &job, std::string &output, sem_t *sema);
    };

    /// @brief Prompt (for muli-thread environment)
    class Prompt
    {
    private:
        std::string const userID;
        LabFSP::FSP_Threaded &multiPrompt;
        std::string const inputFile;
        std::string const outputFile;
        sem_t sem;

        bool logOutput(std::string const &output);

    public:
        Prompt(LabFSP::FSP_Threaded &multiPrompt, const std::string userID, const std::string &inputFile, std::string const &outputFile);

        void run();
    };

}