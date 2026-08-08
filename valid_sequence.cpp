class Solution {
public:
    vector<int> validSequence(string word1, string word2) {
        int n = word1.size();
        int m = word2.size();

        // suf[i] = maximum number of characters of word2
        // that can be matched starting from word1[i]
        vector<int> suf(n + 1, 0);

        int j = m - 1;

        for (int i = n - 1; i >= 0; i--) {
            if (j >= 0 && word1[i] == word2[j]) {
                j--;
            }
            suf[i] = m - 1 - j;
        }

        vector<int> ans;
        int pos = 0;
        bool changed = false;

        for (int k = 0; k < m; k++) {
            bool found = false;

            for (int i = pos; i < n; i++) {
                // If characters are equal, use this index normally.
                if (word1[i] == word2[k]) {
                    // Need enough remaining characters after i
                    if (n - i >= m - k) {
                        ans.push_back(i);
                        pos = i + 1;
                        found = true;
                        break;
                    }
                }

                // Use the one allowed character change.
                if (!changed) {
                    // After changing word1[i], all remaining characters
                    // must be matched exactly.
                    if (suf[i + 1] >= m - k - 1) {
                        ans.push_back(i);
                        pos = i + 1;
                        changed = true;
                        found = true;
                        break;
                    }
                }
            }

            if (!found)
                return {};
        }

        return ans;
    }
};
