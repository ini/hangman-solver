//
//  HangmanSolver.cpp
//  Hangman Solver
//
//  Created by Ini on 8/8/15.
//  Copyright (c) 2015 Insi Productions. All rights reserved.
//

#include "HangmanSolver.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <time.h>


HangmanSolver::HangmanSolver() {
    // Puts words from wordlist.txt into set 'dictionary'
    string word;
    ifstream word_list("wordlist.txt");
    if (word_list.is_open())
    {
        while (getline(word_list, word)) dictionary.insert(word);
        word_list.close();
    }
    
    // Puts words from commonWords.txt into set 'commonWords'
    ifstream common_words("commonWords.txt");
    if (common_words.is_open())
    {
        while (getline(common_words, word)) commonWords.push_back(word);
        common_words.close();
    }
}

// Sets word length and removes any words from 'possibleWords' that aren't the specified length
void HangmanSolver::setWordLength(int length) {
    wordLength = length;
    wordChars = {};
    for (int i = 0; i < wordLength; i++) wordChars.push_back('_');

    for (string word : dictionary) {
        if (word.length() == wordLength) possibleWords.insert(word);
    }
    
    for (string word : commonWords) {
        if (word.length() == wordLength) possibleCommonWords.insert(word);
    }
}

// Returns a map of character counts for words in a set
map<char, int> HangmanSolver::letterCounts(set<string> & wordSet) {
    map<char, int> distribution;
    for (string word : wordSet) {
        set<char> prevChars;
        for (char c : word) {
            if (previousGuesses.count(c) == 0 && prevChars.count(c) == 0) {
                if (distribution.count(c) == 1) distribution[c] += 1;
                else distribution[c] = 1;
                prevChars.insert(c);
            }
        }
    }
    return distribution;
}

// Returns the character to be guessed
char HangmanSolver::guess() {
    // Finds the most frequently occuring character(s) in the set of any possible remaining words
    map<char, int> distribution = letterCounts(possibleWords);
    int bestCharCount = 0;
    vector<char> bestChars;
    
    for (auto &stat : distribution) {
        if (stat.second == bestCharCount) bestChars.push_back(stat.first);
        else if (stat.second > bestCharCount) {
            bestChars.clear();
            bestChars = {stat.first};
            bestCharCount = stat.second;
        }
    }
    
    // Finds the most frequently occurring character in the set of possible common words among the best choices in 'bestChars'
    map<char, int> commonWordsDistribution = letterCounts(possibleCommonWords);
    int bestCommonCharCount = 0;
    vector<char> bestCommonChars;
    
    for (char c : bestChars) {
        if (commonWordsDistribution.count(c) == 1) {
            if (commonWordsDistribution[c] == bestCommonCharCount) bestCommonChars.push_back(c);
            else if (commonWordsDistribution[c] > bestCommonCharCount) {
                bestCommonChars.clear();
                bestCommonChars = {c};
                bestCommonCharCount = commonWordsDistribution[c];
            }
        }
    }
    
    char bestChar = '\0';
    if (bestChars.empty()) return '\0';
    if (bestCommonChars.empty()) {
        // Randomly picks between characters that are equally good choices
        srand(int(time(NULL)));
        bestChar = bestChars[rand() % bestChars.size()];
    }
    else if (bestCommonChars.size() == 1) bestChar = bestCommonChars[0];
    else {
        // Chooses the character whose possible words have the lowest average index in 'commonWords'
        double minAvg = commonWords.size() * commonWords.size();
        for (char c : bestCommonChars) {
            double sum = 0;
            double count = 0;
            for (string word : possibleCommonWords) {
                if (word.find_first_of(c) != string::npos) {
                    sum += find(commonWords.begin(), commonWords.end(), word) - commonWords.begin();
                    count++;
                }
            }
            if (sum / count < minAvg) {
                minAvg = sum / count;
                bestChar = c;
            }
        }
    }
    
    previousGuesses.insert(bestChar);
    return bestChar;
}

// Inputs the letter to given positions in 'wordChars' and adjusts 'possibleWords' accordingly
void HangmanSolver::setGuessResult(char guessedChar, set<int> positions) {
    set<string> tempPossibleWords;
    for (string item : possibleWords) tempPossibleWords.insert(item);
    
    if (positions.empty()) {
        // Removes any words containing an incorrectly guessed letter
        wrongGuesses++;
        for (string word : tempPossibleWords) {
            if (word.find(guessedChar) != string::npos) {
                possibleWords.erase(word);
                possibleCommonWords.erase(word);
            }
        }
    }
    else {
        // Removes any words not containing the correctly guessed letter in the specified positions, or words containing the letter in any positions other than those specified
        for (string word : tempPossibleWords) {
            for (int i = 0; i < wordLength; i++) {
                if (positions.count(i + 1) > 0) {
                    wordChars[i] = toupper(guessedChar);
                    if (word[i] != guessedChar) {
                        possibleWords.erase(word);
                        possibleCommonWords.erase(word);
                    }
                }
                else {
                    if (word[i] == guessedChar) {
                        possibleWords.erase(word);
                        possibleCommonWords.erase(word);
                    }
                }
            }
        }
    }
}

// Prints out the current state of the hangman game, with all letters and blank spaces
void HangmanSolver::printGame() {
    for (char c : wordChars) cout << c << " ";
    cout << endl;
}

// Executes gameplay in console
void HangmanSolver::playGame() {
    // Clears any data from previous games
    previousGuesses = {};
    wrongGuesses = 0;
    cout << "Pick any word." << endl;
    cout << "How many letters are in your word? ";
    int length = 0;
    while (true) {
        cin >> length;
        if (length > 0) break;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please enter a positive integer. ";
    }
    setWordLength(length);

    while (true) {
        printGame();
        char myGuess = guess();
        
        // Tests to see if the hangman game is over
        if (myGuess == '\0') {
            bool wordGuessed = true;
            for (char c : wordChars) {
                if (c == '_') {
                    wordGuessed = false;
                    cout << "Sorry, I must not have your word in my dictionary." << endl;
                    break;
                }
            }
            
            if (wordGuessed) {
                string word;
                for (char ch : wordChars) {
                    word += tolower(ch);
                }
                
                // Cuts the word's index in 'commonWords' in half
                if (find(commonWords.begin(), commonWords.end(), word) != commonWords.end()) {
                    int index = (int) distance(commonWords.begin(), find(commonWords.begin(), commonWords.end(), word));
                    commonWords.erase(remove(commonWords.begin(), commonWords.end(), word), commonWords.end());
                    commonWords.insert(commonWords.begin() + index / 2, word);
                }
                // Inserts the guessed word into middle of 'commonWords'
                else commonWords.insert(commonWords.begin() + commonWords.size() / 2, word);
                
                // Rewrites 'commonWords.txt' with the updated array of words
                ofstream common_words("commonWords.txt");
                for (string w : commonWords) common_words << w << "\n";
                
                if (wrongGuesses == 1) cout << "I got your word with only 1 wrong guess!" << endl;
                else cout << "I got your word with " << wrongGuesses << " wrong guesses!" << endl;
            }
            
            char response;
            cout << "Would you like to start a new game? [Y/N] ";
            cin >> response;
            if (toupper(response) == 'Y') {
                cout << endl;
                playGame();
            }
            break;
        }
        
        // Guesses the word if only one possible word remains
        if (possibleWords.size() == 1) {
            string word;
            for (string w : possibleWords) word = w;
            cout << "Is your word '" << word << "'? [Y/N] ";
            char response;
            cin >> response;
            possibleWords.clear();
            possibleCommonWords.clear();
            map<char, set<int>> positions;
            for (int i = 0; i < wordLength; i++) {
                previousGuesses.insert(word[i]);
                if (toupper(response) != 'N') wordChars[i] = toupper(word[i]);
            }
            continue;
        }
        
        cout << "Is the letter '" << char(toupper(myGuess)) << "' in your word? [Y/N] ";
        char response;
        cin >> response;
        if (toupper(response) == 'Y') {
            // Automatically fills in missing character if only one unguessed character is left
            int unguessedChars = 0;
            int indexOfChar = -1;
            for (int i = 0; i < wordLength; i++) {
                if (wordChars[i] == '_') {
                    unguessedChars++;
                    indexOfChar = i;
                }
            }
            if (unguessedChars == 1) setGuessResult(myGuess, {indexOfChar + 1});
            else {
                // Reads character positions from input
                cin.clear();
                cout << "Enter " << char(toupper(myGuess)) << "'s position(s) in your word: ";
                string line;
                set<int> positions;
                cin.ignore();
                getline(std::cin, line);
                std::stringstream ss(line);
                int i;
                while (ss >> i)
                {
                    positions.insert(i);
                    if (!isdigit(ss.peek())) {
                        ss.ignore();
                    }
                }
                setGuessResult(myGuess, positions);
            }
        }
        else setGuessResult(myGuess, {});
    }
}

// Generates sequence of best character guesses in order for any hangman word
vector<char> HangmanSolver::getListOfGuesses(string word) {
    if (!isWord(word)) {
        cerr << "Error: " << word << " is not in this dictionary." << endl;
    }
    vector<char> guessSequence;
    setWordLength(int(word.length()));
    previousGuesses = {};
    wrongGuesses = 0;
    while (true) {
        char compGuess = guess();
        if (compGuess == '\0') break;
        guessSequence.push_back(compGuess);
        set<int> positions;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == compGuess) positions.insert(i + 1);
        }
        setGuessResult(compGuess, positions);
    }
    return guessSequence;
}

// Returns true if the word is in the dictionary, false otherwise.
bool HangmanSolver::isWord(string word) {
    return dictionary.count(word) != 0;
}