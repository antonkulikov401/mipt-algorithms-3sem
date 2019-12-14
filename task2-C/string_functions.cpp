#include "string_functions.hpp"

void CountingSort(std::vector<size_t>& shifts,
        std::vector<size_t>& classes, std::vector<size_t>& newShifts,
        size_t classesCount, size_t n) {
    std::vector<size_t> count(classesCount);
    for (size_t i = 0; i < n; ++i)
        ++count[classes[newShifts[i]]];
    for (size_t i = 1; i < classesCount; ++i)
        count[i] += count[i - 1];
    for (int64_t i = n - 1; i >= 0; --i)
        shifts[--count[classes[newShifts[i]]]] = newShifts[i];
}

void CalculateClasses(std::vector<size_t>& classes,
        const std::vector<size_t>& shifts, size_t& classesCount,
        size_t n, size_t len, bool firstIteration) {
    std::vector<size_t> newClasses(n);
    classesCount = 1;
    newClasses[0] = 0;
    for (size_t i = 1; i < n; ++i) {
        auto curr = std::make_pair(classes[shifts[i]],
            firstIteration ? 0 : classes[(shifts[i] + len) % n]);
        auto prev = std::make_pair(classes[shifts[i - 1]],
            firstIteration ? 0 : classes[(shifts[i - 1] + len) % n]);
        if (curr != prev) ++classesCount;
        newClasses[shifts[i]] = classesCount - 1;
    }
    classes.swap(newClasses);
}

std::vector<size_t> SortCyclicShifts(const std::string& s,
        const char firstSymbol, const size_t alphabetSize) {
    std::vector<size_t> shifts(s.size());
    std::vector<size_t> classes(s.size());
    std::vector<size_t> newShifts(s.size());

    size_t classesCount = alphabetSize;
    for (size_t i = 0; i < s.size(); ++i) newShifts[i] = i;
    for (size_t i = 0; i < s.size(); ++i) classes[i] = s[i] - firstSymbol;
    CountingSort(shifts, classes, newShifts, classesCount, s.size());
    CalculateClasses(classes, shifts, classesCount, s.size(), 0, true);
    
    for (size_t len = 1; len < s.size(); len *= 2) {
        for (size_t i = 0; i < s.size(); ++i) {
            int64_t newValue = shifts[i] - len;
            if(newValue < 0) newValue += s.size();
            newShifts[i] = newValue;
        }
        CountingSort(shifts, classes, newShifts, classesCount, s.size());
        CalculateClasses(classes, shifts, classesCount, s.size(), len, false);
    }
    return shifts;
}

std::vector<size_t> BuildSuffixArray(const std::string& str) {
    constexpr size_t alphabetSize = 29;
    constexpr char sentinel = '^';
    std::string s = str + sentinel;
    auto suffixArray = SortCyclicShifts(s, sentinel, alphabetSize);
    suffixArray.erase(suffixArray.begin());
    return suffixArray;
}

std::vector<size_t> BuildInverseSuffixArray(
        const std::vector<size_t>& suffixArray) {
    std::vector<size_t> inverseSuffixArray(suffixArray.size());
    for (size_t i = 0; i < suffixArray.size(); ++i)
        inverseSuffixArray[suffixArray[i]] = i;
    return inverseSuffixArray;
}

std::vector<size_t> BuildLCP(const std::string& str,
        const std::vector<size_t>& suffixArray) {
    auto inverseSuffixArray = BuildInverseSuffixArray(suffixArray);
    std::vector<size_t> lcp(str.size() - 1);
    size_t k = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        if (inverseSuffixArray[i] == str.size() - 1) {
            k = 0;
            continue;
        }
        size_t j = suffixArray[inverseSuffixArray[i] + 1];
        while (i + k < str.size() && j + k < str.size() &&
            str[i + k] == str[j + k]) ++k;
        lcp[inverseSuffixArray[i]] = k;
        if (k > 0) --k;
    }
    return lcp;
}

std::string FindKthCommonSubstring(const std::string& first,
        const std::string& second, size_t k) {
    std::string str = first + "`" + second + "_";
    auto suffixArray = BuildSuffixArray(str);
    auto lcp = BuildLCP(str, suffixArray);
    size_t substringCount = 0;
    size_t currMin = 0;
    for (size_t i = 0; i < lcp.size(); ++i) {
        if ((suffixArray[i] <= first.size() &&
                suffixArray[i + 1] <= first.size()) ||
                (suffixArray[i] > first.size() &&
                suffixArray[i + 1] > first.size())) {
            currMin = (lcp[i] < currMin || i == 0) ? lcp[i] : currMin;
            continue;
        }
        substringCount += (lcp[i] > currMin) ? lcp[i] - currMin : 0;
        if (substringCount >= k) {
            size_t len = lcp[i] - (substringCount - k);
            return str.substr(suffixArray[i], len);
        }
        currMin = lcp[i];
    }
    return "";
}
