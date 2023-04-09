#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

class MyObject
{
public:
    int ID;
    string Name;
    vector<int> Array;

    MyObject(int id, const string &name, const vector<int> &array)
        : ID(id), Name(name), Array(array)
    {
        Array.push_back(7896);
    }

    void print() const
    {
        cout << "ID: " << ID << endl;
        cout << "Name: " << Name << endl;
        cout << "Array: ";
        for (const auto &val : Array)
        {
            cout << val << " ";
        }
        cout << endl;
    }
};

// Serialize MyObject to JSON
json to_json(const MyObject &obj)
{
    json j;
    j["ID"] = obj.ID;
    j["Name"] = obj.Name;
    j["Array"] = obj.Array;
    return j;
}

// Deserialize MyObject from JSON
MyObject from_json(const json &j)
{
    return MyObject(j["ID"].get<int>(), j["Name"].get<string>(), j["Array"].get<vector<int>>());
}

// Write MyObject to a JSON file
void write_to_file(const MyObject &obj, const string &filename)
{
    json j = to_json(obj);
    ofstream file(filename);
    file << j.dump(4); // 4 spaces for indentation
    file.close();
}

// Read MyObject from a JSON file
MyObject read_from_file(const string &filename)
{
    ifstream file(filename);
    json j;
    file >> j;
    return from_json(j);
}

int main()
{
    // Create a MyObject instance
    MyObject obj(1, "Sample Object", {1, 2, 3, 4, 5});

    // Write the object to a JSON file
    string filename = "object.json";
    write_to_file(obj, filename);

    // Read the object from the JSON file and print its details
    MyObject obj_from_file = read_from_file(filename);
    obj_from_file.print();

    return 0;
}