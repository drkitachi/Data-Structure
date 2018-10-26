#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

map<pair<string, string>, float> bigram_counts;
map<string, float> unigram_counts;

void calc_bigrams(const string&);
void lookup_bigram(const string&, const string&);
void process(string&);

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: ./calc_bigrams <FILENAME>\n";
		return 0;
	}

    try{
        calc_bigrams(argv[1]);
    }catch(exception e){
        cout << "ERROR: File was not found" << endl;
    }

	vector<pair<string, string>> words = {
		{ "with", "me" },
		{ "the", "grass" },
		{ "the", "king" },
		{ "to", "you" }
	};

	for (auto p : words) {
		lookup_bigram(p.first, p.second);
	}
}

// Given filename fn, read in the file word by word
// for each word:
// 1. process(word)
// 2. increment count of that word in unigram_counts
// 3. increment count of (prevword, word) in bigram_counts
void calc_bigrams(const string& fn) {
    ifstream input(fn);
    if(!input) throw exception();
    string word, prevword;
    pair<string, string> wordPair;

    input >> word;
    process(word);
    while(!input.eof()){
        unigram_counts[word]++;
        prevword = word;
        input >> word;
        process(word);
        wordPair.first = prevword;
        wordPair.second = word;
        bigram_counts[wordPair]++;
    }

    input.close();
}

// Given words w1 and w2,
// 0. process the words
// 1. print the words
// 2. if bigram (w1, w2) is not found, print "Bigram not found"
// 3. print how many times w1 appears
// 4. print how many times (w1, w2) appears
// 5. print count(w1, w2)/count(w1)
// (see example output)
void lookup_bigram(const string& w1, const string& w2) {
    string first = w1, second = w2;
    process(first);
    process(second);
    pair<string, string> wordPair = {first,second};

    if(bigram_counts.find(wordPair) == bigram_counts.end()){
        cout << "Bigram " << first << ", " << second << " not found" << endl;
        return;
    }
    cout << first << ", " << second << ":" << endl;
    cout << first << " appears " << unigram_counts[first] << " times" << endl;
    cout << first << ", " << second << " appears " << bigram_counts[wordPair] << " times" << endl;
    cout << bigram_counts[wordPair] / unigram_counts[first] << endl;
}

void process(string& str) {
	for (int i = 0; i < str.length(); ++i) {
		if(isalpha(str[i])) {
			str[i] = tolower(str[i]);
		}
		else {
			str[i] = '*';
		}
	}
	str.erase(remove(str.begin(), str.end(), '*'), str.end());
}
