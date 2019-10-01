#include "trie.hpp"

void Trie::AddString(const std::string& str, int patternIndex) {
    auto currNode = root;
    for (size_t i = 0; i < str.size(); ++i) {
        size_t symbolIndex = str[i] - 'a';
        bool isTerminal = i == str.size() - 1;
        if (currNode->GetChild(symbolIndex)) {
            currNode = currNode->GetChild(symbolIndex);
            if (isTerminal) currNode->SetPatternIndex(patternIndex);
        }
        else
            currNode = currNode->SetChild(symbolIndex, 
                isTerminal ? patternIndex : -1);
    }
}

void Trie::BFS(void (TrieNode<alphabetSize>::* func)()) {
    std::queue<std::shared_ptr<TrieNode<alphabetSize>>> q;
    q.push(root);
    while (!q.empty()) {
        auto currVertex = q.front();
        q.pop();
        ((*currVertex).*func)();
        for (auto child : currVertex->GetChildren())
            if (child) q.push(child);
    }
}

void Trie::BuildSuffixLinks() {
    BFS(&TrieNode<alphabetSize>::SetSuffixLink);
}

void Trie::BuildDictSuffixLinks() {
    BFS(&TrieNode<alphabetSize>::SetDictSuffixLink);
}

void Trie::NextState(char symbol) {
    size_t symbolIndex = symbol - 'a';
    while (!currState->GetChild(symbolIndex) && currState != root)
        currState = currState->GetSuffixLink();
    currState = currState->GetChild(symbolIndex) ?
        currState->GetChild(symbolIndex) : currState;
}

std::vector<int> Trie::GetPatternIndices() {
    std::vector<int> indices;
    auto currVertex = currState;
    if (currVertex->isTerminal())
        indices.push_back(currVertex->GetPatternIndex());
    while (currVertex->GetDictSuffixLink()) {
        indices.push_back(currVertex->GetDictSuffixLink()->GetPatternIndex());
        currVertex = currVertex->GetDictSuffixLink();
    }
    return indices;
}
