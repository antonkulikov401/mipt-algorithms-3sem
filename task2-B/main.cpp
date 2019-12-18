#include "suffix_tree.hpp"

int main() {
    std::string firstString, secondString;
    std::cin >> firstString >> secondString;
    SuffixTree suffixTree(firstString, secondString);
    std::cout << suffixTree.GetNodeCount() << "\n";
    suffixTree.Print();
    return 0;
}
