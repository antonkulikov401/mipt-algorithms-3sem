#pragma once
#include <vector>
#include <memory>
#include <queue>
#include <functional>
#include <string_view>

template <size_t AlphabetSize>
class TrieNode : public std::enable_shared_from_this<TrieNode<AlphabetSize>> {
public:
    using NodePtr = std::shared_ptr<TrieNode<AlphabetSize>>;
    using WeakNodePtr = std::weak_ptr<TrieNode<AlphabetSize>>;

    TrieNode();
    TrieNode(NodePtr _parent, size_t i, int _patternIndex);
    bool isTerminal() const ;
    std::vector<NodePtr> GetChildren();
    NodePtr GetChild(size_t i);
    NodePtr SetChild(size_t i, int _patternIndex);
    void SetPatternIndex(int _patternIndex);
    int GetPatternIndex() const;
    void CalculateSuffixLinks();
    void CalculateDictSuffixLinks();
    NodePtr GetSuffixLink();
    NodePtr GetDictSuffixLink();

private:
    WeakNodePtr parent;
    WeakNodePtr suffixLink;
    WeakNodePtr dictSuffixLink;
    std::vector<NodePtr> children;
    int patternIndex;
    size_t edgeToParent;
};

class Trie {
public:
    Trie(const std::vector<std::string_view>& patterns);
    void NextState(char symbol);
    std::vector<size_t> GetPatternIndices();

private:
    static const size_t alphabetSize = 26;
    std::shared_ptr<TrieNode<alphabetSize>> root;
    std::shared_ptr<TrieNode<alphabetSize>> currState;

    void BFS(std::function<void(TrieNode<alphabetSize>::NodePtr)> func);
    void AddString(std::string_view str, size_t patternIndex);
    void BuildSuffixLinks();
    void BuildDictSuffixLinks();
};

template<size_t AlphabetSize>
TrieNode<AlphabetSize>::TrieNode() : children(AlphabetSize),
        edgeToParent(0), patternIndex(-1), suffixLink(NodePtr()),
        dictSuffixLink(NodePtr()) {}

template<size_t AlphabetSize>
TrieNode<AlphabetSize>::TrieNode(NodePtr _parent, size_t i,
        int _patternIndex) : children(AlphabetSize), parent(_parent),
        edgeToParent(i), patternIndex(_patternIndex) {}

template<size_t AlphabetSize>
bool TrieNode<AlphabetSize>::isTerminal() const {
    return patternIndex != -1; 
}

template<size_t AlphabetSize>
std::vector<typename TrieNode<AlphabetSize>::NodePtr>
TrieNode<AlphabetSize>::GetChildren() {
    return children;
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::NodePtr
TrieNode<AlphabetSize>::GetChild(size_t i) {
    return children[i];
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::NodePtr
TrieNode<AlphabetSize>::SetChild(size_t i, int _patternIndex) {
    return children[i] = std::make_shared<TrieNode<AlphabetSize>>(
        this->shared_from_this(), i, _patternIndex);
}

template<size_t AlphabetSize>
void TrieNode<AlphabetSize>::SetPatternIndex(int _patternIndex) {
    patternIndex = _patternIndex;
}

template<size_t AlphabetSize>
int TrieNode<AlphabetSize>::GetPatternIndex() const {
    return patternIndex;
}

template<size_t AlphabetSize>
void TrieNode<AlphabetSize>::CalculateSuffixLinks() {
    if (parent.expired()) {
        suffixLink = this->shared_from_this();
        return;
    }
    if (parent.lock()->parent.expired()) {
        suffixLink = parent;
        return;
    }
    auto currVertex = parent.lock()->suffixLink.lock();
    while (!currVertex->children[edgeToParent] &&
            !currVertex->parent.expired())
        currVertex = currVertex->suffixLink.lock();
    suffixLink = currVertex->children[edgeToParent] ?
        currVertex->children[edgeToParent] : currVertex;
}

template<size_t AlphabetSize>
void TrieNode<AlphabetSize>::CalculateDictSuffixLinks() {
    if (parent.expired() || parent.lock()->parent.expired()) {
        dictSuffixLink = NodePtr();
        return;
    }
    auto currVertex = suffixLink.lock();
    while (!currVertex->parent.expired() && !currVertex->isTerminal())
        currVertex = currVertex->suffixLink.lock();
    dictSuffixLink = currVertex->parent.expired() ?
        NodePtr() : currVertex;
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::NodePtr
TrieNode<AlphabetSize>::GetSuffixLink() {
    return suffixLink.lock();
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::NodePtr
TrieNode<AlphabetSize>::GetDictSuffixLink() {
    return dictSuffixLink.lock();
}
