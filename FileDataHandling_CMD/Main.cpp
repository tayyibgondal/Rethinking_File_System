#include "FileManagementSystem.h"
#include <iostream>

using namespace std;

class FileSystemPrompt
{
private:
    LabFMS::FileManagementSystem &FMS;

public:
    FileSystemPrompt(LabFMS::FileManagementSystem &FMS)
        : FMS(FMS) {}

    void Run()
    {
        cout << "Welcome to the File System prompt!" << endl;
        cout << "Type 'help' for a list of available commands." << endl;

        string input;
        while (true)
        {
            cout << "> ";
            getline(cin, input);

            if (input == "help")
            {
                PrintHelp();
            }
            else if (input == "quit")
            {
                // To DO: Save state
                break;
            }
            else
            {
                ExecuteCommand(input);
            }
        }

        cout << "Goodbye!" << endl;
    }

private:
    void PrintHelp()
    {
        cout << "Available commands:" << endl;
        cout << "  Create <fname>" << endl;
        cout << "  Delete <fname>" << endl;
        cout << "  Mkdir <dirName>" << endl;
        // [ ] cout << "  Chdir <dirName>" << endl;
        cout << "  Move <source_fname> <target_fname>" << endl;
        cout << "  Open <fName> <mode:r/w>" << endl;
        cout << "  Close <fName>" << endl;
        cout << "  Write_to_file <filename> <text>" << endl;
        cout << "  Write_to_file <filename> <text> <startLocation>" << endl;
        cout << "  Read_from_file <filename> <start> <size>" << endl;
        cout << "  Truncate_file <filename> <maxSize>" << endl;
        cout << "  SMM" << endl;
        cout << "  Help" << endl;
        cout << "  Quit" << endl;
    }

    void ExecuteCommand(const string &input)
    {
        string command;
        string arg1, arg2, arg3, arg4;
        istringstream iss(input);
        iss >> command >> arg1 >> arg2 >> arg3 >> arg4;

        if (command == "Create")
        {
            cout << "Result: " << FMS.createFile(arg1) << endl;
        }
        else if (command == "Delete")
        {
            cout << "Result: " << FMS.deleteFile(arg1) << endl;
        }
        else if (command == "Mkdir")
        {
            cout << "Result: " << FMS.createDir(arg1) << endl;
        }
        else if (command == "Chdir")
        {
            cout << "fileSystemObject.ChangeDirectory(arg1);";
        }
        else if (command == "Move")
        {
            cout << "Result: " << FMS.moveFile(arg1, arg2) << endl;
        }
        else if (command == "Open")
        {
            if (arg2 == "r")
            {
                cout << "Result: " << FMS.openFile(arg1, 'r') << endl;
            }
            else if (arg2 == "w")
            {
                cout << "Result: " << FMS.openFile(arg1, 'w') << endl;
            }
            else
            {
                cerr << "Invalid file mode: " << arg2 << endl;
            }
        }
        else if (command == "Close")
        {
            cout << "Result: " << FMS.closeFile(arg1) << endl;
        }
        else if (command == "Write_to_file")
        {
            if (arg3.empty())
            {
                cout << "Result: " << FMS.write_to_file(arg1, arg2) << endl;
            }
            else
            {
                int start = stoi(arg3);
                cout << "Result: " << FMS.write_to_file(arg1, arg2, start) << endl;
            }
        }
        else if (command == "Read_from_file")
        {
            if (arg2.empty())

                cout << "Result: " << FMS.read_from_file(arg1) << endl;
            else
            {
                int start = stoi(arg2);
                int size = stoi(arg3);
                cout << "Result: " << FMS.read_from_file(arg1, start, size) << endl;
            }
        }
        else if (command == "Truncate_file")
        {
            int maxSize = stoi(arg2);
            cout << "fileSystemObject.TruncateFile" << FMS.truncate_file(arg1, maxSize) << endl;
        }
        else if (command == "SMM")
        {
            cout << FMS.Show_memory_map();
        }
        else if (command == "Help")
        {
            PrintHelp();
        }
        else if (command == "Quit")
        {
            exit(0);
        }
        else
        {
            cerr << "Invalid command: " << command << endl;
        }
    }
};

int main(int argc, char const *argv[])
{
    LabSH::storageHardwareConst sysConst(100, 10, 20, 10, 10);
    LabSH::storageHardware SSD(sysConst);
    LabFMS::FileManagementSystem FMS(SSD);
    FileSystemPrompt f(FMS);
    f.Run();
}