#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<DirectoryFunctions*> directory_objects;
vector<FileFunctions*> file_objects;

class DirectoryFunctions;
class FileFunctions;

class FileFunctions {
public:
    string path;
    FileFunctions(string path) {
        this->path = path;
    }
    FileFunctions() {
        this->path = "";
    }
    string toString() {
        return path;
    }
};

class DirectoryFunctions {
public:
    vector<int> directory_indices;
    vector<int> file_indices;
    string path;
    DirectoryFunctions(string path) {
        this->path = path;
    }
    DirectoryFunctions() {
        this->path = "root";
    }

    string open(string dirName, string currentPath) {
        return currentPath + "/" + dirName;
    }

    void list_files_and_directories(string currentPath) {
        // find the directory object from global list of directories (using path sent by user prompt class)
        DirectoryFunctions* directory_object;
        for (int i = 0; i < directory_objects.size(); i++) {
            if (directory_objects[i]->toString() == currentPath) {
                directory_object = directory_objects[i];
                break;
            }
        }
        // Print all directories
        for (int i = 0; i < directory_object->directory_indices.size(); i++) {
            cout << directory_objects[directory_object->directory_indices[i]]->toString() << " (directory)" << endl;
        }
        // Print all files
        for (int i = 0; i < directory_object->file_indices.size(); i++) {
            cout << file_objects[directory_object->file_indices[i]]->toString() << " (file)" << endl;
        }
    }

    void mkdir(string name, string curr_path) {
        // Create new directory object
        string complete_path = curr_path + "/" + name;
        DirectoryFunctions* directory;
        directory = new DirectoryFunctions(complete_path);

        // Add it to global list of directories
        int i;
        for (i = 0; i < directory_objects.size(); i++) {
            if (directory_objects[i] == NULL) {
                directory_objects[i] = directory;
                break;
            }
        }
        if (i == directory_objects.size()) {
            directory_objects.push_back(directory);
        }

        // Update directory indices list of source directory
        for (int j = 0; j < directory_objects.size(); j++) {
            if (directory_objects[j]->toString() == curr_path) {
                directory_objects[j]->directory_indices.push_back(i);
            }
        }
    }

    
        void rmdir(string name, string path) {
            // the current directory's path is 'path'
            // the directory to be deleted is 'name'
            string complete_path = path + "/" + name;
            // deleting directory form global list of directories
            int temp = -1;
            for (int i = 0; i < directory_objects.size(); i++) {
                if (directory_objects[i]->toString() == complete_path) {
                    directory_objects[i] = NULL;
                    temp = i;
                    break;
                }
            }
            
            // Updating directory indices of directory which contained the deleted directory
            for (int i = 0; i < directory_objects.size(); i++) {
                if (directory_objects[i]->toString() == path) {  // i.e., parent directory
                    for (int j = 0; j < directory_objects[i]->directory_indices.size(); j++) {
                        if (directory_objects[i]->directory_indices[j] == temp) {
                            directory_objects[i]->directory_indices.erase(directory_objects[i]->directory_indices.begin() + j);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        string chdir(string dirPath) {
            return dirPath;
        }

        void move(string src_name, string curr_path, string dest_path) {
            // Moves file from current directory to dest directory
            string src_path = curr_path + "/" + src_name;
            int temp_idx = -1;

            // find file object index
            for (int i = 0; i < file_objects.size(); i++) {
                if (file_objects[i]->toString() == src_path) {
                    temp_idx = i;
                    break;
                }
            }

            // Now move this index from one directory's file indices list to the destination directory's file indices list

            // Removing index form src directory
            for (int i = 0; i < directory_objects.size(); i++) {
                if (directory_objects[i]->toString() == curr_path) {
                    for (int j = 0; j < directory_objects[i]->file_indices.size(); j++) {
                        if (directory_objects[i]->file_indices[j] == temp_idx) {
                            directory_objects[i]->file_indices.erase(directory_objects[i]->file_indices.begin() + j);
                            break;
                        }
                    }
                    break;
                }
            }
            
            // adding index to destination directory
            for (int i = 0; i < directory_objects.size(); i++) {
                if (directory_objects[i]->toString() == dest_path) {
                    directory_objects[i]->file_indices.push_back(temp_idx);
                    break;
                }
            }
        }

        
        void create_file(string name, string curr_path) {
            // Create new file object
            string complete_path = curr_path + "/" + name;
            FileFunctions* file = new FileFunctions(complete_path);

            // add it to global list of files
            int i;
            for (i = 0; i < file_objects.size() + 1; i++) {
                if (i < file_objects.size() && file_objects[i] == nullptr) {
                    file_objects[i] = file;
                    break;
                }
            }
            if (i == file_objects.size()) {
                file_objects.push_back(file);
            }

            // Update file indices list of current directory
            // Update directory indices list of source directory
            for (DirectoryFunctions* directory_object : directory_objects) {
                if (directory_object->toString() == curr_path) {
                    directory_object->file_indices.push_back(i);
                    break;
                }
            }
        }

        void delete_file(string name, string curr_path) {
            // delete new file object

            // 'name' is the name of file to be deleted
            // 'curr_path' is path of current directory
            string complete_path = curr_path + "/" + name;

            // find form global list of files and remove
            int temp = -1;
            for (int i = 0; i < file_objects.size(); i++) {
                if (file_objects[i]->toString() == complete_path) {
                    delete file_objects[i];
                    file_objects[i] = nullptr;
                    temp = i;
                    break;
                }
            }

            // find the directory which contained this file
            // and remove the index of file to be deleted from
            // its list of file indices
            for (DirectoryFunctions* dir_object : directory_objects) {
                if (dir_object->toString() == curr_path) {
                    for (int i = 0; i < dir_object->file_indices.size(); i++) {
                        if (dir_object->file_indices[i] == temp) {
                            dir_object->file_indices.erase(dir_object->file_indices.begin() + i);
                            break;
                        }
                    }
                    break;
                }
            }
        }

    DirectoryFunctions* getRootDirectory(std::vector<DirectoryFunctions*> directory_objects) {
        DirectoryFunctions* root = nullptr;
        for (DirectoryFunctions* directory_obj : directory_objects) {
            if (directory_obj->toString() == "root") {
                root = directory_obj;
                break;
            }
        }
        return root;
    }

    void showMemoryMap() {
        // implementation here
    }

    void saveSystemState() {
        // implementation here
    }

        string toString() {
            return path;
        }
};

class UserPrompt {
private:
    DirectoryFunctions directory_funcs;
    FileFunctions file_funcs;
    std::string curr_dir;
    std::string curr_file;
public:
    UserPrompt() : directory_funcs("root"), file_funcs(nullptr), curr_dir("root"), curr_file("") {
        int exit_status = 1;
        while (exit_status != -1) {
            exit_status = directory_selection_prompt();
        }
    }
    int directory_selection_prompt() {
        std::cout << "Choose an action:\n";
        std::cout << "0. Quit\n";
        std::cout << "1. List contents\n";
        std::cout << "2. Open directory\n";
        std::cout << "3. Make directory\n";
        std::cout << "4. Change directory\n";
        std::cout << "5. Move file\n";
        std::cout << "6. Select file\n";
        std::cout << "7. Create file\n";
        std::cout << "8. Show memory map\n";
        std::cout << "9. Delete file\n";
        std::cout << "10. Delete Directory\n";

        std::string choice;
        std::cin >> choice;

        if (choice == "0") {
            // directory_funcs.save_system_state();
            std::exit(0);
        }
        else if (choice == "1") { // list contents
            std::cout << "\nContents of current directory: \n";
            directory_funcs.list_files_and_directories(curr_dir);
        }
        else if (choice == "2") { // open directory
            std::string dirName;
            std::cout << "Enter directory name: ";
            std::cin >> dirName;
            curr_dir = directory_funcs.open(dirName, curr_dir);
            std::cout << "Directory opened\n";
        }
        else if (choice == "3") { // make directory
            std::string dirName;
            std::cout << "Enter directory name: ";
            std::cin >> dirName;
            directory_funcs.mkdir(dirName, curr_dir);
            std::cout << "Directory Created\n";
        }
        else if (choice == "4") { // change directory
            std::string dirPath;
            std::cout << "Enter directory path: ";
            std::cin >> dirPath;
            curr_dir = directory_funcs.chdir(dirPath);
            std::cout << "Directory Changed\n";
        }
        else if (choice == "5") { // move file
            std::string srcName, destPath;
            std::cout << "Enter source file name: ";
            std::cin >> srcName;
            std::cout << "Enter destination path: ";
            std::cin >> destPath;
            directory_funcs.move(srcName, curr_dir, destPath);
            std::cout << "File moved\n";
        }
        else if (choice == "6") { // select a file
            std::string file;
            std::cout << "Enter file name: ";
            std::cin >> file;
            curr_file = file;

            int exit_status = 1;
            while (exit_status != -1) {
                exit_status = file_selection_prompt();
            }
        }

        else if (choice == "7") {  // create new file
            std::string fName;
            std::cout << "enter name of the file: ";
            std::cin >> fName;
            directory_funcs.create_file(fName, curr_dir);
        } else if (choice == "8") {  // show memory map
            std::cout << "Memory map is: ";
            // directory_funcs.show_memory_map(); 
        } else if (choice == "9") {
            std::string fName;
            std::cout << "Enter file name: ";
            std::cin >> fName;
            directory_funcs.delete_file(fName, curr_dir);
        } else if (choice == "10") {
            std::string dirName;
            std::cout << "Enter directory name: ";
            std::cin >> dirName;
            directory_funcs.rmdir(dirName, curr_dir);
        }
        return 1;
    }

    int file_selection_prompt() {
    std::cout << "Choose an action:\n";
    std::cout << "0. Deselect file\n";
    std::cout << "1. Open file\n";
    std::cout << "2. Truncate file\n";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "0") {
        return -1;
    } else if (choice == "1") {  // open file
        std::string mode;
        std::cout << ">Enter mode(r for read, w for write): ";
        std::getline(std::cin, mode);

        if (mode == "r") {
            std::cout << "File opened in read mode!\n";
            int exit_status = 1;
            while (exit_status != -1) {
                exit_status = read_mode_prompt();
            }
        } else if (mode == "w") {
            std::cout << "File opened in write mode!\n";
            int exit_status = 1;
            while (exit_status != -1) {
                exit_status = write_mode_prompt();
            }
        }
    } else if (choice == "2") {  // truncate
        std::string maxSize;
        std::cout << "Enter max size for the file: ";
        std::getline(std::cin, maxSize);
        // file_funcs.turncate(curr_file, maxSize);
        std::cout << "file truncated!\n";
    }
    return 1;
}

int write_mode_prompt() {
    std::cout << "Choose an action:\n";
    std::cout << "1. Write to file(append)\n";
    std::cout << "2. Write to file at position\n";
    std::cout << "3. Move within file\n";
    std::cout << "4. Close file\n";

    std::string choice;
    std::getline(std::cin, choice);

    if (choice == "4") {
        std::cout << "file closed!\n";
        return -1;
    }

    if (choice == "1") {  // append data to the file
        std::string text;
        std::cout << "enter text: ";
        std::getline(std::cin, text);
        // file_funcs.append(curr_file, text);
        std::cout << "appended!\n";
    } else if (choice == "2") {  // add data at specified position
        std::string text;
        std::cout << "enter text: ";
        std::getline(std::cin, text);
        std::string position;
        std::cout << "enter position: ";
        std::getline(std::cin, position);
        // file_funcs.write_at_position(curr_file, text, position);
        std::cout << "data entered at specified position!\n";
    } else if (choice == "3") {  // move within file
        std::string frm, to, siz;
        std::cout << "from: ";
        std::getline(std::cin, frm);
        std::cout << "to: ";
        std::getline(std::cin, to);
        std::cout << "size: ";
        std::getline(std::cin, siz);
        // file_funcs.move_within_file(curr_file, frm, to, siz);
        std::cout << "moved!\n";
    }
}

    int read_mode_prompt() {
    std::cout << "Choose an action:" << std::endl;
    std::cout << "1. Read from file" << std::endl;
    std::cout << "2. Close the file" << std::endl;

    std::string choice;
    std::cin >> choice;

    if (choice == "1") {  // read from file
        std::string start, end;
        std::cout << "Enter start position: ";
        std::cin >> start;
        std::cout << "Enter ending position: ";
        std::cin >> end;
        std::cout << "Contents of file are: " << std::endl;
        // file_funcs.read_file(curr_file, start, end);
        std::cout << std::endl;
        return 1;
    }
    else if (choice == "2") {  // close file
        std::cout << "file closed!" << std::endl;
        return -1;
    }

    return 1;
}

};

int main() {
    // Global lists of directory and file objects
    directory_objects = {new DirectoryFunctions("root")};
    UserPrompt();
    return 1;

}




