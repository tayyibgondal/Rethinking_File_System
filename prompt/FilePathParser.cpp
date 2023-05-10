#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main()
{
    std::string input = "/Dir1/Dir2/file1.txt";
    input.erase(0, 1);

    // Create a stringstream from the input string
    std::stringstream ss(input);

    // Create a vector to store the split strings
    std::vector<std::string> result;

    // Split the input string using '/' as the delimiter
    while (ss.good())
    {
        std::string substr;
        getline(ss, substr, '/');
        result.push_back(substr);
    }
    int n = result.size();
    // Convert the vector to an array of strings
    std::string arr[result.size()];
    int m = arr->size();
    for (int i = 0; result.size() > 0; i++)
    {
        arr[i] = result[0];
        result.erase(result.begin());
    }

    // Print the array to verify the result
    for (int i = 0; i < arr->size(); i++)
    {
        std::cout << "--" << arr[i] << std::endl;
    }
    int m = arr->size();

    return 0;
}
