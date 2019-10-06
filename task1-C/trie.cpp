#include "trie.hpp"

Trie::Trie(const std::vector<std::string>& patterns) :
        root(std::make_shared<TrieNode<alphabetSize>>()), currState(root) {
    for (size_t i = 0; i < patterns.size(); ++i)
        AddString(patterns[i], i);
    BuildSuffixLinks();
    BuildDictSuffixLinks();
}

void Trie::AddString(const std::string& str, size_t patternIndex) {
    auto currNode = root;
    for (size_t i = 0; i < str.size(); ++i) {
        size_t symbolIndex = str[i] - 'a';
        bool isTerminal = i == str.size() - 1;
        if (currNode->GetChild(symbolIndex)) {
            currNode = currNode->GetChild(symbolIndex);
            if (isTerminal) currNode->SetPatternIndex(patternIndex);
            continue;
        }
        currNode = currNode->SetChild(symbolIndex, 
            isTerminal ? patternIndex : -1);
    }
}

void Trie::BFS(std::function<void(TrieNode<alphabetSize>::NodePtr)> func) {
    std::queue<TrieNode<alphabetSize>::NodePtr> q;
    q.push(root);
    while (!q.empty()) {
        auto currVertex = q.front();
        q.pop();
        func(currVertex);
        for (auto child : currVertex->GetChildren())
            if (child) q.push(child);
    }
}

void Trie::BuildSuffixLinks() {
    BFS([](TrieNode<alphabetSize>::NodePtr v) {
        v->CalculateSuffixLinks();
    });
}

void Trie::BuildDictSuffixLinks() {
    BFS([](TrieNode<alphabetSize>::NodePtr v) {
        v->CalculateDictSuffixLinks();
    });
}

void Trie::NextState(char symbol) {
    size_t symbolIndex = symbol - 'a';
    while (!currState->GetChild(symbolIndex) && currState != root)
        currState = currState->GetSuffixLink();
    currState = currState->GetChild(symbolIndex) ?
        currState->GetChild(symbolIndex) : currState;
}

std::vector<size_t> Trie::GetPatternIndices() {
    std::vector<size_t> indices;
    auto currVertex = currState;
    if (currVertex->isTerminal())
        indices.push_back(currVertex->GetPatternIndex());
    while (currVertex->GetDictSuffixLink()) {
        indices.push_back(currVertex->GetDictSuffixLink()->GetPatternIndex());
        currVertex = currVertex->GetDictSuffixLink();
    }
    return indices;
}
