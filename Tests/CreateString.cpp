#include <sstream>
#include <string>
#include <iostream>

typedef enum : char
{
    LabInode = 'I',
    LabFile = 'F',
    LabDir = 'D',
    LabBlock = 'B'
} nodeType;

struct InodeEntry
{
    std::string const name;
    int const inode; // inode ID

    InodeEntry(std::string const &name, int const &inode)
        : name(name), inode(inode) {}

    std::string toString() const
    {
        std::stringstream ss;
        ss << "{name: \"" << name << "\", \ninode: " << LabBlock << "}" << std::endl;
        return ss.str();
    }
};

int main(int argc, char const *argv[])
{
    InodeEntry ie("Abdul", 123);

    std::cout << ie.toString() << "****" << std::endl;
    return 0;
}
