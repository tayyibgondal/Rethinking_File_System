#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

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
// std::vector<std::string> *inputFiles = getThreadFileNames(threadCount, "input");
std::vector<std::string> *getFileNames(int threadCount)
{
    // check enough arguments
    if (threadCount <= 0)
        return nullptr;

    std::vector<std::string> inputFiles = {"input_thread1.txt", "input_thread<2>.txt", "input3.txt"};
    ;
    for (int count = 0; count < threadCount; count++)
    {
        std::string fileName = inputFiles.at(count);

        ifstream file(fileName);
        cout << "Opening: " << fileName << endl;
        if (!file.is_open())
        {
            cout << "Error: " << fileName << " does not exist" << endl;
            return nullptr;
        }
        file.close();
    }

    return nullptr;
}
int main(int argc, char const *argv[])
{
    std::vector<std::string> inputFiles = {"input_thread<0>.txt", "input_thread<1>.txt", "input_thread<2>.txt", "input<3>.txt"};
    std::vector<std::string> *tFiles = getThreadFileNames(4, "input");

    for (int i = 0; i < 4; i++)
    {
        cout << (inputFiles.at(i) == tFiles->at(i)) << endl;
        cout << inputFiles.at(i) << " - " << tFiles->at(i) << endl;
    }

    return 0;
}
