#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // for parsing
using namespace std;

class FileSystemPrompt
{
public:
    FileSystemPrompt() {}

    void Run(const string& filename)
{
    ifstream infile(filename);
    if (!infile)
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    cout << "==============================================" << endl;
    cout << "Welcome to the File System prompt!" << endl;
    cout << "Type 'help' for a list of available commands." << endl;
    cout << "==============================================" << endl;

    string input;
    while (getline(infile, input))
    {
        cout << endl;
        cout << "> " << input << endl;

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

    cout << endl;
    cout << "Goodbye!" << endl;
}

private:
    void PrintHelp()
    {
        cout << "Available commands:" << endl;
        cout << "  Create <fname>" << endl;
        cout << "  Delete <fname>" << endl;
        cout << "  Mkdir <dirName>" << endl;
        cout << "  Chdir <dirName>" << endl;
        cout << "  Move <source_fname, target_fname>" << endl;
        cout << "  Open <fName,mode>" << endl;
        cout << "  Close <fName>" << endl;
        cout << "  Write_to_file <filename>, text" << endl;
        cout << "  Write_to_file <filename>,text, startLocation, size" << endl;
        cout << "  Read_from_file <filename,start,size" << endl;
        cout << "  Truncate_file <filename, maxSize>" << endl;
        cout << "  Show_memory_map" << endl;
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
            cout << "fileSystemObject.CreateFile(arg1);";
        }
        else if (command == "Delete")
        {
            cout << "fileSystemObject.DeleteFile(arg1);";
        }
        else if (command == "Mkdir")
        {
            cout << "fileSystemObject.CreateDirectory(arg1);";
        }
        else if (command == "Chdir")
        {
            cout << "fileSystemObject.ChangeDirectory(arg1);";
        }
        else if (command == "Move")
        {
            cout << "fileSystemObject.MoveFile(arg1, arg2);";
        }
        else if (command == "Open")
        {
            if (arg2 == "read")
            {
                cout << "fileSystemObject.OpenFile(arg1, r);";
            }
            else if (arg2 == "write")
            {
                cout << "fileSystemObject.OpenFile(arg1, w);";
            }
            else
            {
                cerr << "Invalid file mode: " << arg2 << endl;
            }
        }
        else if (command == "Close")
        {
            cout << "fileSystemObject.CloseFile(arg1);";
        }
        else if (command == "Write_to_file")
        {
            if (arg4.empty())
            {
                cout << "fileSystemObject.WriteToFile(arg1, arg2);";
            }
            else
            {
                int start = stoi(arg3);
                int size = stoi(arg4);
                cout << "fileSystemObject.WriteToFile(arg1, arg2, start, size);";
            }
        }
        else if (command == "Read_from_file")
        {
            int start = stoi(arg2);
            int size = stoi(arg3);
            cout << "fileSystemObject.ReadFromFile(arg1, start, size);";
        }
        else if (command == "Truncate_file")
        {
            int maxSize = stoi(arg2);
            cout << "fileSystemObject.TruncateFile(arg1, maxSize);";
        }
        else if (command == "Show_memory_map")
        {
            cout << "fileSystemObject.ShowMemoryMap();";
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

class outToFile {
public: 
    outToFile() {}

    void appendStringToFile(const std::string& str, const std::string& filename) {
        // Open the file for appending
        std::ofstream outfile(filename, std::ios::app);

        // Write the string to the file
        outfile << str;

        // Close the file
        outfile.close();
    }
};

int main()
{
    FileSystemPrompt f = FileSystemPrompt();
    f.Run("file.txt");
}