

#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <cassert>

#include "BinaryTree.hpp"


int main()
{
    cg::BinaryTree<int, int, cg::Less<int>> t;

    t.Emplace(7, 123);
    t.Emplace(6, 456);
    t.Emplace(8, 789);
    t.Emplace(9, 012);
    t.Emplace(4, 345);
    t.Emplace(3, 678);
    t.Emplace(10, 901);

    t.ShowKeys(std::cout);

    int stop = 0;
    return 0;
}
