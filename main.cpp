#include <iostream>
#include <set>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
 
 
using namespace std;
 
const int MIN_LETTERS = 4;
 
void load_dictionary(ifstream& in_file, set<string>& dictionary);
string to_lower_case(string word);
void load_board(ifstream& in_file, vector<string>& board_letters, vector<vector<string>>& board);
void print_board(const vector<vector<string>>& board, ofstream& out_file, const vector<string>& board_letters);
bool visited(vector<vector<int>>& is_visited, int r, int c);
void find_matched_words(const vector<vector<string>>& board, const set<string>& dictionary, ofstream& out_file, const int size, vector<vector<int>> is_visited, set <string>& matched_words);
bool in_bounds (int size, int r, int c);
bool search_dictionary(const set<string>& dictionary, string word, set<string>& matched_words);
void recursive_search(vector<vector<int>>& is_visited, const vector<vector<string>>& board, int size, int r, int c, const set<string>& dictionary, string prefix, set <string>& matched_words);
void initialize_board(vector<vector<int>>& is_visited, const vector<vector<string>>& board);
void print_matched_words(set<string>& matched_words, ofstream& out_file);
 
 
 
 
int main(int argc, char* argv[]) {
 
 
    vector<string> board_letters;
 
    vector<vector<string>> board;
 
    set<string>dictionary;
 
    vector<vector<int>> is_visited;
 
    set <string> matched_words;
 
    ifstream in_file;
 
    in_file.open(argv[1]);
 
    load_dictionary(in_file, dictionary);
 
    in_file.close();
 
    in_file.open(argv[2]);
 
    load_board(in_file, board_letters, board);
 
    in_file.close();
 
    ofstream out_file;
 
    int size = sqrt(board_letters.size());
 
    find_matched_words(board, dictionary, out_file, size, is_visited, matched_words);
 
    out_file.open(argv[3]);
 
    print_board(board, out_file, board_letters);
 
    print_matched_words(matched_words, out_file);
 
    out_file.close();
 
    return 0;
}
void load_dictionary(ifstream& in_file, set<string>& dictionary)
{
    string word;
    while(getline(in_file, word))
    {
        dictionary.insert(to_lower_case(word));
    }
 
}
string to_lower_case(string word)
{
    string lower_case_word = word;
    for (unsigned r = 0; r < word.length(); r++)
    {
        lower_case_word[r] = tolower(word[r]);
    }
    return lower_case_word;
}
void load_board(ifstream& in_file, vector<string>& board_letters, vector<vector<string>>& board)
{
    string line;
    while(getline(in_file, line))
    {
        stringstream strm;
        strm << line;
        string letter;
        while(strm >> letter) {
            board_letters.push_back(to_lower_case(letter));
        }
    }
 
    int sqrt_of_board_size = sqrt(board_letters.size());
 
        int position = 0;
        for (auto r = 0; r < sqrt_of_board_size; r++)
        {
            vector<string> temp;
            for (auto c = 0; c < sqrt_of_board_size; c++) {
                temp.push_back(board_letters[position]);
                position++;
            }
 
            board.push_back(temp);
        }
}
void print_board(const vector<vector<string>>& board, ofstream& out_file, const vector<string>& board_letters)
{
    int sqrt_of_board_size = sqrt(board_letters.size());
    for (auto r = 0; r < sqrt_of_board_size; r++)
    {
        for (auto c = 0; c < sqrt_of_board_size; c++) {
 
            out_file << board[r][c] << " ";
        }
        out_file << "\n";
 
    }
}
bool visited(vector<vector<int>>& is_visited, int r, int c)
{
    if (is_visited[r][c] == 1)
    {
        return true;
    }
    else{
        is_visited[r][c] = 0;
        return false;
    }
 
}
void find_matched_words(const vector<vector<string>>& board, const set<string>& dictionary, ofstream& out_file, const int size, vector<vector<int>> is_visited, set <string>& matched_words)
{
    string prefix;
    initialize_board(is_visited, board);
    for (auto r = 0; r < size; r++)
    {
        for (auto c = 0; c < size; c++)
        {
            recursive_search(is_visited, board, size, r, c, dictionary, prefix, matched_words);
            initialize_board(is_visited, board);
        }
    }
}
bool in_bounds(int size, int r, int c)
{
 
    if (r >= size || r < 0 || c >= size || c < 0)
    {
        return false;
    }
    return true;
}
bool search_dictionary(const set<string>& dictionary, string word,  set <string>& matched_words)
{
    set<string>::iterator i = dictionary.lower_bound(word);
    if (i == dictionary.end()) {
        return false;
    }
 
    string possible_prefix = *i;
    if (word == possible_prefix.substr(0, word.length())) {
        if (word == possible_prefix && word.length() >= MIN_LETTERS)
        {
            matched_words.insert(word);
            return true;
        }
        return true;
    }
        return false;
 
 
}
void recursive_search(vector<vector<int>>& is_visited, const vector<vector<string>>& board, int size, int r, int c, const set<string>& dictionary, string prefix, set <string>& matched_words)
{
    if (in_bounds(size, r, c))
    {
        if (!visited(is_visited, r, c))
        {
            prefix += board[r][c];
            is_visited[r][c] = 1;
            if (!search_dictionary(dictionary, prefix, matched_words))
            {
                is_visited[r][c] = 0;
                return;
            }
 
            recursive_search(is_visited, board, size, r+1, c, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r, c+1, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r+1, c+1, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r-1, c, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r, c-1, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r-1, c-1, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r-1, c+1, dictionary, prefix, matched_words);
            recursive_search(is_visited, board, size, r+1, c-1, dictionary, prefix, matched_words);
            is_visited[r][c] = 0;
        }
    }
}
 
void print_matched_words(set<string>& matched_words, ofstream& out_file)
{
    out_file << "\n";
    for (auto m : matched_words)
    {
        out_file << m << "\n";
    }
}
void initialize_board(vector<vector<int>>& is_visited, const vector<vector<string>>& board)
{
    vector<int> initial_board;
    is_visited.clear();
    for(unsigned r = 0; r < board.size(); r++)
    {
        for (unsigned c = 0; c < board.size(); c++)
        {
            initial_board.push_back(0);
        }
        is_visited.push_back(initial_board);
    }
    initial_board.clear();
 
}
