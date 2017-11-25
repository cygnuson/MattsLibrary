

#include <iostream>
#include <string>
#include <queue>
#include <cstdlib>
#include <cassert>

#include "BinaryTree.hpp"


int main()
{
    cg::BinaryTree<std::string, int, cg::Less<int>> t;
    t.Emplace(100, "100");
    t.Emplace(200, "200");
    t.Emplace(250, "250");
    t.Emplace(50, "50");
    
    t.ShowKeys(std::cout);
    std::cout << "\n";
    t.Get(200) = "200.";
    t.Pop(200);
    t.ShowKeys(std::cout);

    std::cout << "\n\n\n";
    auto beg = t.RBegin();
    auto end = t.REnd();
    for (; beg != end; ++beg)
        std::cout << (*beg).m_a << ",";

    int stop = 0;
    return 0;
}