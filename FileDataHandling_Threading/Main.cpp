#include "FileSystemPrompt.h"
#include <iostream>

int main(int argc, char const *argv[])
{
    LabSH::storageHardwareConst sysConst(100, 10, 20, 10, 10);
    LabSH::storageHardware SSD(sysConst);
    LabFMS_MU::FileManagementSystem FMS(SSD);
    LabFSP::FileSystemPrompt FSP(FMS);
    LabFSP::FileSystemPrompt_MU FSP_MU(FMS);
    LabFSP::FSP_CMD f(FSP);
    LabFSP::FSP_CMD f2(FSP_MU);
    f.run();
    std::cout << "\nMulti-user mode started!\n";
    f2.run();
}
