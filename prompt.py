import sys
from fileFunctions import FileFunctions

# global lists of directory and file objects
directory_objects = []
file_objects = []

class DirectoryFunctions:
    def __init__(self, path):
        self.directory_indices = []
        self.file_indices = []
        self.path = path

    def open(dirName, currentPath):
        return currentPath + '/' + dirName

    def list_files_and_directories(currentPath):
        # find the directory object from global list of 
        # directories (using path sent by user prompt class)
        for directory_object in directory_objects:
            if directory_object.__str__() == currentPath:
                break
        # Print all directores 
        for directory_idx in directory_object.directory_indices:
            print(directory_objects[directory_idx].__str__())
        # Print all files
        for file_idx in directory_object.file_indices:
            print(file_objects[file_idx].__str__())

    def mkdir(self, name, curr_path):
        # Create new directory object
        complete_path = curr_path + '/' + name
        directory = DirectoryFunctions(complete_path)

        # add it to global list of files
        index = 0
        for directory_object in directory_objects:
            if directory_object == None:
                directory_objects[index] = directory
                break
            index += 1
        if index == len(file_objects):
            directory_objects.append(directory)

        # Update file indices list of current directory
        self.directory_indices.append(index)

    def rmdir(self, name, path):
        complete_path = path + '/' + name
        # deleting directory form global list of directories
        temp = None
        for i in range(0, len(directory_objects)):
            if directory_objects[i].__str__() == complete_path:
                directory_objects[i] = None
                temp = i
                break
        
        # Updating directory indices of directory which contained the deleted directory
        for i in range(0, len(directory_objects)):
            if directory_objects[i].__str__() == path:
                for j in range(0, directory_objects[i].directory_indices):
                    if directory_objects[i].directory_indices[j] == temp:
                        del directory_objects[i].directory_indices[j]
                        break
                break
        
    def chdir(dirPath):
        return dirPath

    def move(src_name, curr_path, dest_path):
        # Moves file from current directory to dest directory
        src_path = curr_path + '/' + src_name
        temp_idx = None

        # find file object index
        index = 0
        for file_object in file_objects:
            if file_object.__str__() == src_path:
                temp_idx = index
            index += 1
        # Now move this index from one directory's file 
        # indices list to the destination directory's file
        #  indices list
        
        # Removing index form src directory
        for directory_object in directory_objects:
            if directory_object.__str__() == curr_path:
                for i in range(0, len(directory_object.file_indices)):
                    if directory_object.file_indices[i] == temp_idx:
                        del directory_object.file_indices[i]
                        break
                break 
        # adding index to destination directory
        for directory_object in directory_objects:
            if directory_object.__str__() == dest_path:
                directory_object.file_indices.append(temp_idx)
                break
        

    def create_file(self, name, curr_path):
        # Create new file object
        complete_path = curr_path + '/' + name
        file = FileFunctions(complete_path)

        # add it to global list of files
        index = 0
        for file_object in file_objects:
            if file_object == None:
                file_objects[index] = file
                break
            index += 1

        if index == len(file_objects):
            file_objects.append(file)

        # Update file indices list of current directory
        self.file_indices.append(index)

    def delete_file(self, name, curr_path):
        # delete new file object
        complete_path = curr_path + '/' + name

        # find form global list of files and remove
        index = 0
        for file_object in file_objects:
            if file_object.__str__() == complete_path:
                file_objects[index] = None
                break
            index += 1

        # find the directory which contained this file
        # and remove the index of file to be deleted from
        # its list of file indices
        for dir_object in directory_objects:
            if dir_object.__str__() == curr_path:
                for i in range(0, len(dir_object.file_indices)):
                    if dir_object.file_indices[i] == index:
                        del dir_object.file_indices[i] 
                        break
                break

        # Update file indices list of current directory
        self.file_indices.append(index)

    def show_memory_map():
        pass

    def __str__(self):
        return self.path

class UserPrompt:
    # =========================================
    # constructor
    # =========================================
    def __init__(self):
        self.directory_funcs = DirectoryFunctions()
        self.file_funcs = FileFunctions()
        self.curr_dir = None
        self.curr_file = None
        # prompting user
        exit_status = 1
        while exit_status != -1:
            exit_status = self.directory_selection_prompt()
        
    # =========================================
    # Prompt when a directory is opened
    # =========================================
    def directory_selection_prompt(self):
        print("Choose an action:")
        print("0. Quit")
        print("1. List contents")
        print("2. Open directory")
        print("3. Make directory")
        print("4. Change directory")
        print("5. Move file")
        print("6. Select file")
        print("7. Create file")
        print("8. Show memory map")
        print("9. Delete file")
        print("10. Delete Directory")

        choice = input("> ")

        if choice == "0":
            # self.directory_funcs.save_system_state()
            sys.exit()

        elif choice == "1":  # list contents
            print("\nContents of current directory: ")
            self.directory_funcs.list_files_and_directories(self.curr_dir)

        elif choice == "2": # open directory
            dirName = input("Enter directory name: ")
            self.curr_dir = self.directory_funcs.open(dirName, self.curr_dir)
            print("Directory opened")

        elif choice == "3": # make directory
            dirName = input("Enter directory name: ")
            self.directory_funcs.mkdir(dirName, self.curr_dir)
            print("Directory Created")

        elif choice == "4": # change directory
            dirPath = input("Enter directory path: ")
            self.curr_dir = self.directory_funcs.chdir(dirPath) 
            print("Directory Changed")

        elif choice == "5": # move file
            srcName = input("Enter source file name: ")
            destPath = input("Enter destination path: ")
            # self.directory_funcs.move(srcName, self.curr_dir, destPath)
            print("file moved")

        elif choice == "6":  # select a file
            file = input("enter file name ")
            self.curr_file = file

            exit_status = 1
            while exit_status != -1:
                exit_status = self.file_selection_prompt()   

        elif choice == "7":  # create new file
            fName = input("enter name of the file: ")
            # self.directory_funcs.create_file(fName, self.curr_dir)

        elif choice == "8":  # show memory map
            print("Memory map is: ")
            # self.directory_funcs.show_memory_map() 
        
        elif choice == "9":
            fName = input("Enter file name: ")
            self.directory_funcs.delete_file(fName, self.curr_dir)

        elif choice == "10":
            dirName = input("Enter directory name: ")
            self.directory_funcs.rmdir(dirName, self.curr_dir)

        return 1

    # =========================================
    # Prompt when a file is selected from the directory
    # =========================================
    def file_selection_prompt(self):
        print("Choose an action:")
        print("0. Deselect file")
        print("1. Open file")
        print("2. Truncate file")

        choice = input("> ")

        if choice == "0":
            return -1

        elif choice == "1": # open file
            mode = input(">Enter mode(r for read, w for write): ")

            if mode == 'r':
                print("File opened in read mode!")
                exit_status = 1
                while exit_status != -1:
                    exit_status = self.read_mode_prompt()   
                      
            elif mode == 'w':
                print("File opened in write mode!")
                exit_status = 1
                while exit_status != -1:
                    exit_status = self.write_mode_prompt()

        elif choice == "2":  # truncate
            maxSize = input("Enter max size for the file: ")
            # self.file_funcs.turncate(self.curr_file, maxSize)   
            print("file truncated!")
     
        return 1 

    # =========================================
    # prompt when the selected file is opened in write mode
    # =========================================
    def write_mode_prompt(self):
        print("Choose an action:")
        print("1. Write to file(append)")
        print("2. Write to file at position")
        print("3. Move within file")
        print("4. Close file")

        choice = input("> ")

        if choice == "4":
            print("file closed!")
            return -1
        
        if choice == "1":  # append data to the file
            text = input("enter text: ")
            # self.file_funcs.append(self.curr_file, text)
            print('appended!')
        
        elif choice == "2":  # add data at specified position
            text = input("enter text: ")
            position = input("enter position: ")
            # self.file_funcs.write_at_position(self.curr_file, text, position)
            print('data entered at specified position!')
        
        elif choice == "3":  # move within file
            frm = input("from: ")
            to = input("to: ")
            siz = input("size: ")
            # self.file_funcs.move_within_file(self.curr_file, frm, to, siz)
            print("moved! ")

        return 1
        
    # =========================================
    # prompt when the selected file is opened in read mode
    # =========================================
    def read_mode_prompt(self):
        print("Choose an action:")
        print("1. Read from file")
        print("2. Close the file")

        choice = input("> ")

        if choice == "1":  # read form file
            start = input("Enter start position: ")
            end = input("Enter ending position ")
            print("Contents of file are: ")
            # self.file_funcs.read_file(self.curr_file, start, end)
            print("\n")
            return 1
        
        elif choice == "2": # close file
            print("file closed!")
            return -1

UserPrompt()
           