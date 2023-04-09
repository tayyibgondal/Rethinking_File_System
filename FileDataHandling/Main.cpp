#include <iostream>
#include <fstream>
#include <string>
#include "storageHardware.h"
using namespace std;

int main(int argc, char const *argv[])
{
    LabSH::storageHardware pc(5, 3);
    pc.writeBlock(0, "12345");
    pc.writeBlock(1, "ABCDE");
    pc.writeBlock(2, "DA\nSH");
    cout << pc.readBlock(0) << endl;
    cout << pc.readBlock(1) << endl;
    cout << pc.readBlock(2) << endl;

    LabSH::StorageHardware::write_to_file(pc, "WorkFile.json");

    std::ifstream workFile("WorkFile.json");

    auto pc2 = LabSH::StorageHardware::build_from_file(workFile);

    workFile.close();

    cout << pc2.readBlock(0) << endl;
    cout << pc2.readBlock(1) << endl;
    cout << pc2.readBlock(2) << endl;

    return 0;
}
