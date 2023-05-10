// range-based-for.cpp
// compile by using: cl /EHsc /nologo /W4
#include <iostream>
#include <vector>
using namespace std;

int main()
{
    // Basic 10-element integer array.
    int x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Range-based for loop to iterate through the array.
    for (int y : x)
    { // Access by value using a copy declared as a specific type.
      // Not preferred.
        cout << y << " ";
    }
    cout << endl;

    // The auto keyword causes type inference to be used. Preferred.

    for (auto y : x)
    { // Copy of 'x', almost always undesirable
        cout << y << " ";
    }
    cout << endl;

    for (auto &y : x)
    { // Type inference by reference.
        // Observes and/or modifies in-place. Preferred when modify is needed.
        cout << y << " ";
    }
    cout << endl;

    for (const auto &y : x)
    { // Type inference by const reference.
        // Observes in-place. Preferred when no modify is needed.
        cout << y << " ";
    }
    cout << endl;
    cout << "end of integer array test" << endl;
    cout << endl;

    // Create a vector object that contains 10 elements.
    vector<double> v;
    for (int i = 0; i < 10; ++i)
    {
        v.push_back(i + 0.14159);
    }

    // Range-based for loop to iterate through the vector, observing in-place.
    for (double &j : v)
    {
        cout << --j << " ";
    }
    cout << endl;
    cout << "end of vector test" << endl;
}