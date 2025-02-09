#include "StringUtils.h"

namespace StringUtils
{

    std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept
    {
        int len = str.length();
        int slice_len;

        // Account for negative indices for start
        if (start < 0)
        {
            start = len + start;
        }
        
        // Account for the start index being negative and invalid
        if (start < 0)
        {
            return "";
        }

        // Account for end index being negative
        if (end < 0)
        {
            end = len + end;
        }

        // Account for the end index being negative and invalid
        if (end < 0)
        {
            return "";
        }

        // Account for the end index being invalid due to being greater than the length of the string
        if (end > len)
        {
            return "";
        }

        // Account for the end index being 0, then just return the string from the start index to the end
        if (end == 0)
        {
            return str.substr(start);
        }

        // Account for the end index being less than the start index
        if (start > end)
        {
            return "";
        }

        return str.substr(start, end - start);
    }

    std::string Capitalize(const std::string &str) noexcept
    {
        int len = str.length();
        std::string capString;
        if (len == 0)
        {
            return "";
        }
        // Capitalize the first character
        capString = std::toupper(str[0]);
        // Ensure the rest of the string is lowercase
        capString += Lower(str.substr(1));
        return capString;
    }

    std::string Upper(const std::string &str) noexcept
    {
        std::string upperString;
        for (char ch : str)
        {
            upperString += std::toupper(ch);
        }
        return upperString;
    }

    std::string Lower(const std::string &str) noexcept
    {
        std::string lowerString;
        for (char ch : str)
        {
            lowerString += std::tolower(ch);
        }
        return lowerString;
    }

    std::string LStrip(const std::string &str) noexcept
    {
        int len = str.length();
        int i;
        for (i = 0; i < len; i++)
        {
            if (!std::isspace(str[i]))
            {
                break;
            }
        }
        // i is the first non-whitespace character
        return str.substr(i);
    }

    std::string RStrip(const std::string &str) noexcept
    {
        int len = str.length();
        int i;
        // Iterate backwards through the string
        for (i = len - 1; i > -1; i--)
        {
            if (!std::isspace(str[i]))
            {
                break;
            }
        }
        // i is the first non-whitespace character
        return str.substr(0, i + 1);
    }

    std::string Strip(const std::string &str) noexcept
    {
        return LStrip(RStrip(str));
    }

    std::string Center(const std::string &str, int width, char fill) noexcept
    {
        int len = str.length();
        int num_fill = width - len;
        if (num_fill > 0)
        {
            int left_fill = num_fill / 2;
            int right_fill = num_fill - left_fill; // In case num_fill is odd
            return std::string(left_fill, fill) + str + std::string(right_fill, fill);
        }
        else
        {
            return str;
        }
    }

    std::string LJust(const std::string &str, int width, char fill) noexcept
    {
        int len = str.length();
        int num_fill = width - len;
        if (num_fill > 0)
        {
            return str + std::string(num_fill, fill);
        }
        else
        {
            return str;
        }
    }

    std::string RJust(const std::string &str, int width, char fill) noexcept
    {
        int len = str.length();
        int num_fill = width - len;
        if (num_fill > 0)
        {
            return std::string(num_fill, fill) + str;
        }
        else
        {
            return str;
        }
    }

    std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept
    {
        int len_str = str.length();
        int len_old = old.length();
        std::string new_str;
        std::vector<int> old_indices; // To store the indices where the old string is found
        for (int i = 0; i < len_str; i++)
        {
            if (str.substr(i, len_old) == old)
            {
                old_indices.push_back(i);
                // In the case of repeated characters, we don't want multiple replacements
                i += len_old - 1; // -1 because the loop will increment i by 1
            }
        }

        int num_matches = old_indices.size();

        if (num_matches == 0)
        {
            return str;
        }

        // First part of the string before the first match
        new_str = str.substr(0, old_indices[0]);

        // Iterate through the matches and append the parts of the string in between
        for (int i = 0; i < num_matches - 1; i++)
        {
            new_str += rep;
            new_str += str.substr(old_indices[i] + len_old, old_indices[i + 1] - old_indices[i] - len_old);
        }

        // Append the last part of the string after the last match
        new_str += rep;
        new_str += str.substr(old_indices[num_matches - 1] + len_old);

        return new_str;
    }

    std::vector<std::string> Split(const std::string &str, const std::string &splt) noexcept
    {
        std::vector<std::string> out_strings;
        int len = str.length();
        std::string temp;

        // If splt is empty, then split by whitespace
        if (splt == "")
        {
            for (int i = 0; i < len; i++)
            {
                if (!isspace(str[i]))
                {
                    temp += str[i];
                }
                else
                {
                    // Account for multiple whitespaces
                    if (temp != "")
                    {
                        out_strings.push_back(temp); // Add the non-empty substr to the vector
                        temp = "";
                    }
                }
            }
            // Account for the last substring
            if (temp != "")
            {
                out_strings.push_back(temp);
            }
            return out_strings;
        }
        else
        {
            int splt_len = splt.length();
            for (int i = 0; i < len; i++)
            {
                if (str.substr(i, splt_len) != splt)
                {
                    temp += str[i];
                }
                else
                {
                    out_strings.push_back(temp);
                    temp = "";
                    i += splt_len - 1; // -1 because the loop will increment i by 1
                }
            }
            if (temp != "")
            {
                out_strings.push_back(temp);
            }
            return out_strings;
        }
    }

    std::string Join(const std::string &str, const std::vector<std::string> &vect) noexcept
    {
        std::string outstring;
        for (int i = 0; i < vect.size(); i++)
        {
            outstring += vect[i];
            // Separate the strings with the str
            // Don't add the str after the last string
            if (i != vect.size() - 1)
            {
                outstring += str;
            }
        }
        return outstring;
    }

    std::string ExpandTabs(const std::string &str, int tabsize) noexcept
    {
        std::string outstring;
        // If tabsize is less than 1, then just remove the tabs
        if (tabsize < 1)
        {
            for (char c : str)
            {
                if (!(c == '\t'))
                {
                    outstring += c;
                }
            }
            return outstring;
        }
        // Otherwise, replace the tabs with spaces
        for (char c : str)
        {
            if (!(c == '\t'))
            {
                outstring += c;
            }
            else
            {
                int curr_len = outstring.length();
                // Calculate the number of spaces needed to reach the next tabstop for alignment
                int num_spaces = tabsize - (outstring.length() % tabsize);
                outstring += std::string(num_spaces, ' ');
            }
        }
        return outstring;
    }

    int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept
    {
        int n = left.length();
        int m = right.length();

        // Create a 2D array to store the distances
        // This is a vector of vectors
        std::vector<std::vector<int>> distances_arr(n + 1, std::vector<int>(m + 1));

        // Initialize the first row and column
        for (int i = 0; i < n + 1; i++)
        {
            distances_arr[i][0] = i;
        }
        for (int j = 0; j < m + 1; j++)
        {
            distances_arr[0][j] = j;
        }

        // Fill in the rest of the array
        // Iterate across the rows
        for (int i = 1; i < n + 1; i++)
        {
            // Iterate across the columns
            for (int j = 1; j < m + 1; j++)
            {
                int sub_cost, del_cost, ins_cost;
                if (ignorecase)
                {
                    // Need to do -1 since we added an extra row and column
                    if (std::tolower(left[i - 1]) == std::tolower(right[j - 1]))
                    {
                        sub_cost = distances_arr[i - 1][j - 1];
                    }
                    else
                    {
                        sub_cost = distances_arr[i - 1][j - 1] + 1;
                    }
                    del_cost = distances_arr[i - 1][j] + 1;
                    ins_cost = distances_arr[i][j - 1] + 1;
                    distances_arr[i][j] = std::min({sub_cost, del_cost, ins_cost});
                }
                else
                {
                    if (left[i - 1] == right[j - 1])
                    {
                        sub_cost = distances_arr[i - 1][j - 1] + 0;
                    }
                    else
                    {
                        sub_cost = distances_arr[i - 1][j - 1] + 1;
                    }
                    del_cost = distances_arr[i - 1][j] + 1;
                    ins_cost = distances_arr[i][j - 1] + 1;
                    distances_arr[i][j] = std::min(sub_cost, std::min(del_cost, ins_cost));
                }
            }
        }
        // Display the distances array for testing purposes
        // for (int i = 0; i < n + 1; i++) {
        //     for (int j = 0; j < m + 1; j++) {
        //         std::cout << distances_arr[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        return distances_arr[n][m];
    }

};