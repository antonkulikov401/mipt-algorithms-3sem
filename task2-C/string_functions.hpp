#include <string>
#include <vector>

std::vector<size_t> BuildSuffixArray(const std::string& str);
std::vector<size_t> BuildLCP(const std::string& str,
        const std::vector<size_t>& suffixArray);
std::string FindKthCommonSubstring(const std::string& first,
    const std::string& second, size_t k);
