#include "suffix_tree.hpp"

SuffixTree::SuffixTree(const std::string& first, const std::string& second) :
        root(std::make_shared<SuffixTreeNode>(-1,
            std::make_shared<int32_t>(-1), alphabetSize)),
        str(first + second),
        activeNode(root),
        activeLen(0),
        activeEdge(0),
        leafEnd(new int32_t(-1)),
        remainder(0),
        nextSuffixLink(nullptr),
        firstStringLen(first.size()) {
    Construct();
    int32_t index = -1;
    SetIndices(root, -1, index);
    nodeCount = index + 1;
}

void SuffixTree::Construct() {
    for (size_t i = 0; i < str.size(); ++i)
        AddNode(i);
}

void SuffixTree::AddNode(size_t i) {
    *leafEnd = i + 1;
    ++remainder;
    nextSuffixLink = nullptr;
    size_t strNumber = i > firstStringLen - 1;
    while (remainder > 0) {
        if (activeLen == 0) activeEdge = i;
        if (!activeNode->children[IndexToCode(activeEdge)])
            AddLeaf(i, strNumber);
        else {
            auto nextNode = activeNode->children[IndexToCode(activeEdge)];
            if (activeLen >= nextNode->Length()) {
                activeEdge += nextNode->Length();
                activeLen -= nextNode->Length();
                activeNode = nextNode;
                continue;
            }
            if (str[nextNode->start + activeLen] == str[i]) {
                ++activeLen;
                AddSuffixLink(activeNode);
                break;
            }
            Split(nextNode, i, strNumber);
        }
        --remainder;
        UpdateActivePoint(i);
    }
}

void SuffixTree::AddSuffixLink(std::shared_ptr<SuffixTreeNode> node) {
    if(nextSuffixLink) nextSuffixLink->suffixLink = node;
    nextSuffixLink = node;
}

void SuffixTree::AddLeaf(size_t i, size_t strNumber) {
    activeNode->children[IndexToCode(activeEdge)] =
        std::make_shared<SuffixTreeNode>(i, leafEnd,
        alphabetSize, strNumber);
    AddSuffixLink(activeNode);
}

void SuffixTree::UpdateActivePoint(size_t i) {
    if (activeNode == root && activeLen > 0) {
        --activeLen;
        activeEdge = i - remainder + 1;
    }
    else activeNode = activeNode->suffixLink.expired() ?
        root : activeNode->suffixLink.lock();
}

void SuffixTree::Split(std::shared_ptr<SuffixTreeNode> nextNode,
        size_t i, size_t strNumber) {
    auto split = std::make_shared<SuffixTreeNode>(nextNode->start,
        std::make_shared<int32_t>(nextNode->start + activeLen),
        alphabetSize, nextNode->strNumber);
    activeNode->children[IndexToCode(activeEdge)] = split;
    auto leaf = std::make_shared<SuffixTreeNode>(i, leafEnd,
        alphabetSize, strNumber);
    split->children[IndexToCode(i)] = leaf;
    nextNode->start += activeLen;
    split->children[IndexToCode(nextNode->start)] = nextNode;
    AddSuffixLink(split);
}

void SuffixTree::SetIndices(std::shared_ptr<SuffixTreeNode> node,
        int32_t parentIndex, int32_t& index) {
    node->index = ++index;
    if (node->strNumber == 0 && *node->end > firstStringLen)
        node->end = std::make_shared<int32_t>(firstStringLen);
    if (node->strNumber == 1) {
        node->start -= firstStringLen;
        node->end = std::make_shared<int32_t>(*node->end - firstStringLen);
    }
    for (auto child : node->children)
        if (child) SetIndices(child, node->index, index);
}

void SuffixTree::PrintDFS(std::shared_ptr<SuffixTreeNode> node,
        int32_t parentIndex) {
    if (node != root) std::cout << parentIndex << " " << node->strNumber
        << " " << node->start << " " << *node->end << "\n";
    for (auto child : node->children)
        if (child) PrintDFS(child, node->index);
}

void SuffixTree::Print() {
    PrintDFS(root, -1);
}

size_t SuffixTree::IndexToCode(size_t i) {
    char ch = str[i];
    return ch == '#' ? 0 : ch == '$' ? 1 : ch - 'a' + 2;
}

size_t SuffixTree::GetNodeCount() const {
    return nodeCount;
}
