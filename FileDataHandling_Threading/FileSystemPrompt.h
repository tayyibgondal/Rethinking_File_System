#include "FileManagementSystem.h"

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
}