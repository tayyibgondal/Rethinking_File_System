#include <iostream>
#include <vector>

class Base
{
public:
    int n;
    // virtual ~Base() {} // Ensure that the base class has a virtual destructor
    // virtual void print() const = 0; // Declare a pure virtual function
};

class Derived1 : public Base
{
public:
    void print()
    { // Implement the virtual function
        std::cout << "This is Derived1\n";
    }
};

class Derived2 : public Base
{
public:
    void print() const
    { // Implement the virtual function
        std::cout << "This is Derived2\n";
    }
};

int main()
{
    Base *b1 = new Derived1, *b2 = new Derived2; // Declare a vector of pointers to the base class

    Derived1 *d1p = dynamic_cast<Derived1 *>(b1);
}