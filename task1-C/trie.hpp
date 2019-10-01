#pragma once
#include <vector>
#include <memory>
#include <queue>

template <size_t AlphabetSize>
class TrieNode : public std::enable_shared_from_this<TrieNode<AlphabetSize>> {
public:
    using TriePtr = std::shared_ptr<TrieNode<AlphabetSize>>;
    using WeakTriePtr = std::weak_ptr<TrieNode<AlphabetSize>>;

    TrieNode();
    TrieNode(TriePtr _parent, size_t i, int _patternIndex);
    bool isTerminal() const ;
    std::vector<TriePtr> GetChildren();
    TriePtr GetChild(size_t i);
    TriePtr SetChild(size_t i, int _patternIndex);
    void SetPatternIndex(int _patternIndex);
    int GetPatternIndex() const;
    void SetSuffixLink();
    void SetDictSuffixLink();
    TriePtr GetSuffixLink();
    TriePtr GetDictSuffixLink();

private:
    WeakTriePtr parent;
    WeakTriePtr suffixLink;
    WeakTriePtr dictSuffixLink;
    std::vector<TriePtr> children;
    int patternIndex;
    size_t edgeToParent;
};

class Trie {
public:
    Trie() : root(std::make_shared<TrieNode<alphabetSize>>()),
        currState(root) {}
    void AddString(const std::string& str, int patternIndex);
    void BuildSuffixLinks();
    void BuildDictSuffixLinks();
    void NextState(char symbol);
    std::vector<int> GetPatternIndices();

private:
    static const size_t alphabetSize = 26;
    std::shared_ptr<TrieNode<alphabetSize>> root;
    std::shared_ptr<TrieNode<alphabetSize>> currState;

    void BFS(void (TrieNode<alphabetSize>::* func)());
};

template<size_t AlphabetSize>
TrieNode<AlphabetSize>::TrieNode() : edgeToParent(0), patternIndex(-1),
        suffixLink(TriePtr()), dictSuffixLink(TriePtr()) {
    children.resize(AlphabetSize);
}

template<size_t AlphabetSize>
TrieNode<AlphabetSize>::TrieNode(TriePtr _parent, size_t i,
        int _patternIndex) : parent(_parent), edgeToParent(i),
        patternIndex(_patternIndex) {
    children.resize(AlphabetSize);
}

template<size_t AlphabetSize>
bool TrieNode<AlphabetSize>::isTerminal() const {
    return patternIndex != -1; 
}

template<size_t AlphabetSize>
std::vector<typename TrieNode<AlphabetSize>::TriePtr>
TrieNode<AlphabetSize>::GetChildren() {
    return children;
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::TriePtr
TrieNode<AlphabetSize>::GetChild(size_t i) {
    return children[i];
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::TriePtr
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
void TrieNode<AlphabetSize>::SetSuffixLink() {
    if (parent.expired())
        suffixLink = this->shared_from_this();
    else if (parent.lock()->parent.expired())
        suffixLink = parent.lock();
    else {
        auto currVertex = parent.lock()->suffixLink.lock();
        while (!currVertex->children[edgeToParent] &&
                !currVertex->parent.expired())
            currVertex = currVertex->suffixLink.lock();
        suffixLink = currVertex->children[edgeToParent] ?
            currVertex->children[edgeToParent] : currVertex;
    }
}

template<size_t AlphabetSize>
void TrieNode<AlphabetSize>::SetDictSuffixLink() {
    if (parent.expired() || parent.lock()->parent.expired())
        dictSuffixLink = TriePtr();
    else {
        auto currVertex = suffixLink.lock();
        while (!currVertex->parent.expired() && !currVertex->isTerminal())
            currVertex = currVertex->suffixLink.lock();
        dictSuffixLink = currVertex->parent.expired() ?
            TriePtr() : currVertex;
    }
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::TriePtr
TrieNode<AlphabetSize>::GetSuffixLink() {
    return suffixLink.lock();
}

template<size_t AlphabetSize>
typename TrieNode<AlphabetSize>::TriePtr
TrieNode<AlphabetSize>::GetDictSuffixLink() {
    return dictSuffixLink.lock();
}
