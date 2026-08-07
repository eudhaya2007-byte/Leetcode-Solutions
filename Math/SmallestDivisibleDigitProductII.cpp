#include <string>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
    // Returns counts of prime factors {2, 3, 5, 7} for a digit x (1 to 9)
    vector<int> getFactors(int x) {
        vector<int> f(4, 0);
        while (x % 2 == 0) { f[0]++; x /= 2; }
        while (x % 3 == 0) { f[1]++; x /= 3; }
        while (x % 5 == 0) { f[2]++; x /= 5; }
        while (x % 7 == 0) { f[3]++; x /= 7; }
        return f;
    }

    // Generates the minimal string of digits needed to fulfill the required prime factor counts
    string getMinString(const vector<long long>& need) {
        long long c2 = max(0LL, need[0]);
        long long c3 = max(0LL, need[1]);
        long long c5 = max(0LL, need[2]);
        long long c7 = max(0LL, need[3]);

        string res = "";
        // 7s and 5s are prime and can't be combined with other digits
        res.append(c7, '7');
        res.append(c5, '5');

        // Combine pairs of 3s into 9s
        res.append(c3 / 2, '9');
        c3 %= 2;

        // Combine triplets of 2s into 8s
        res.append(c2 / 3, '8');
        c2 %= 3;

        // Handle remaining combinations of 2s and 3s optimal for minimum length & value
        if (c2 == 2 && c3 == 1) {
            // factors: 2, 2, 3 -> digits 2, 6 (length 2)
            res += "26";
        } else if (c2 == 1 && c3 == 1) {
            res += '6';
        } else if (c2 == 2 && c3 == 0) {
            res += '4';
        } else if (c2 == 1 && c3 == 0) {
            res += '2';
        } else if (c2 == 0 && c3 == 1) {
            res += '3';
        }

        sort(res.begin(), res.end());
        return res;
    }

public:
    string smallestNumber(string num, long long t) {
        // Step 1: Factorize t into prime factors 2, 3, 5, 7
        vector<long long> need(4, 0);
        long long temp = t;
        int primes[4] = {2, 3, 5, 7};
        for (int i = 0; i < 4; ++i) {
            while (temp % primes[i] == 0) {
                need[i]++;
                temp /= primes[i];
            }
        }
        // If t contains prime factors other than 2, 3, 5, 7, it's impossible
        if (temp > 1) return "-1";

        int n = num.length();

        // Step 2: Compute prefix remaining needs and find first zero in num
        vector<vector<long long>> prefix_need(n + 1, need);
        int first_zero = -1;

        for (int i = 0; i < n; ++i) {
            if (num[i] == '0') {
                first_zero = i;
                break;
            }
            vector<int> f = getFactors(num[i] - '0');
            for (int k = 0; k < 4; ++k) {
                prefix_need[i + 1][k] = prefix_need[i][k] - f[k];
            }
        }

        // Check if num itself is valid (no zeros and satisfies target product)
        if (first_zero == -1) {
            bool satisfied = true;
            for (int k = 0; k < 4; ++k) {
                if (prefix_need[n][k] > 0) {
                    satisfied = false;
                    break;
                }
            }
            if (satisfied) return num;
        }

        // Step 3: Backtrack from right to left to find the smallest valid number of same length
        int limit = (first_zero == -1) ? n - 1 : first_zero;

        for (int i = limit; i >= 0; --i) {
            int start_digit = (num[i] - '0') + 1;
            if (i == first_zero) {
                start_digit = 1; // Replace 0 with at least 1
            }

            for (int d = start_digit; d <= 9; ++d) {
                vector<int> f = getFactors(d);
                vector<long long> rem_need(4);
                for (int k = 0; k < 4; ++k) {
                    rem_need[k] = prefix_need[i][k] - f[k];
                }

                string min_suffix = getMinString(rem_need);
                int rem_len = n - 1 - i;

                if ((int)min_suffix.length() <= rem_len) {
                    // Valid string formed! Construct the final answer.
                    string ans = num.substr(0, i);
                    ans += to_string(d);
                    ans.append(rem_len - min_suffix.length(), '1'); // Pad with '1's
                    ans += min_suffix;
                    return ans;
                }
            }
        }

        // Step 4: If no solution of length n exists, find the minimal solution of length > n
        string min_str = getMinString(need);
        int target_len = max((int)min_str.length(), n + 1);
        string ans = "";
        ans.append(target_len - min_str.length(), '1');
        ans += min_str;
        return ans;
    }
};
