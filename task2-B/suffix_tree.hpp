#pragma once
#include <string>
#include <memory>
#include <vector>
#include <iostream>

class SuffixTree {
public:
    SuffixTree(const std::string& first, const std::string& second);
    size_t GetNodeCount() const;
    void Print();

private:
    struct SuffixTreeNode;

    const size_t alphabetSize = 28;
    std::shared_ptr<SuffixTreeNode> root;
    std::string str;
    std::shared_ptr<SuffixTreeNode> activeNode;
    size_t activeLen;
    size_t activeEdge;
    std::shared_ptr<int32_t> leafEnd;
    size_t remainder;
    std::shared_ptr<SuffixTreeNode> nextSuffixLink;
    size_t firstStringLen;
    size_t nodeCount;

    size_t IndexToCode(size_t i);
    void Construct();
    void AddNode(size_t i);
    void AddSuffixLink(std::shared_ptr<SuffixTreeNode> node);
    void Split(std::shared_ptr<SuffixTreeNode> nextNode,
        size_t i, size_t strNumber);
    void AddLeaf(size_t i, size_t strNumber);
    void UpdateActivePoint(size_t i);
    void SetIndices(std::shared_ptr<SuffixTreeNode> node,
        int32_t parentIndex, int32_t& index);
    void PrintDFS(std::shared_ptr<SuffixTreeNode> node,
        int32_t parentIndex);
};

struct SuffixTree::SuffixTreeNode {
    SuffixTreeNode(int32_t _start, std::shared_ptr<int32_t> _end,
        size_t alphabetSize, size_t _strNumber = 0) :
        start(_start),
        end(_end),
        children(alphabetSize),
        index(-1),
        strNumber(_strNumber) {}
    
    int32_t Length() const { return *end - start; }

    int32_t start;
    std::shared_ptr<int32_t> end;
    std::vector<std::shared_ptr<SuffixTreeNode>> children;
    std::weak_ptr<SuffixTreeNode> suffixLink;
    int32_t index;
    size_t strNumber;
};
