#include <iostream>

int main(int argc, char *argv[])
{
    // argc contains the number of command line arguments
    // argv is an array of C-style strings containing the actual arguments

    // Check if there are any command line arguments
    if (argc < 2)
    {
        std::cout << "No command line arguments provided." << std::endl;
        return 0;
    }

    // Access individual command line arguments
    std::cout << "Command line arguments:" << std::endl;
    for (int i = 1; i < argc; i++)
    {
        std::cout << "Argument " << i << ": " << argv[i] << std::endl;
    }

    return 0;
}
