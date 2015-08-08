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

// Puts words from wordlist.txt into set 'dictionary'
HangmanSolver::HangmanSolver() {
    string word;
    ifstream myfile("wordlist.txt");
    if (myfile.is_open())
    {
        while (getline(myfile, word))
        {
            word.erase(remove(word.begin(), word.end(), '\r'), word.end());
            dictionary.insert(word);
        }
        myfile.close();
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
    
}

// Returns a map of character counts for words in 'possibleWords'
map<char, int> HangmanSolver::letterCounts() {
    map<char, int> distribution;
    for (string word : possibleWords) {
        set<char> wordChars;
        for (char& c : word) {
            if (wordChars.count(c) == 0 && previousGuesses.count(c) == 0) {
                if (distribution.count(c) == 1) distribution[c] += 1;
                else distribution[c] = 1;
            }
            wordChars.insert(c);
        }
    }
    return distribution;
}

// Returns the character to be guessed
char HangmanSolver::guess() {
    map<char, int> distribution = letterCounts();
    int maxCharCount = 0;
    vector<char> maxChars = {0};
    
    for (auto &stat : distribution) {
        if (stat.second == maxCharCount) {
            maxChars.push_back(stat.first);
        }
        else if (stat.second > maxCharCount) {
            maxChars.clear();
            maxChars = {stat.first};
            maxCharCount = stat.second;
        }
    }
    
    // Randomly picks between characters that are equally good choices
    srand(int(time(NULL)));
    char maxChar = maxChars[rand() % maxChars.size()];
    previousGuesses.insert(maxChar);
    return maxChar;
}

// Inputs the letter to given positons and adjusts 'possibleWords' accordingly
void HangmanSolver::setGuessResult(char guessedChar, vector<int> positions) {
    set<string> tempPossibleWords;
    for (string item : possibleWords) tempPossibleWords.insert(item);
    
    if (positions.empty()) {
        wrongGuesses++;
        for (string word : tempPossibleWords) {
            if (word.find(guessedChar) != string::npos) possibleWords.erase(word);
        }
    }
    else {
        for (int pos : positions) {
            wordChars[pos - 1] = toupper(guessedChar);
            for (string word : tempPossibleWords) {
                if (word[pos - 1] != guessedChar) possibleWords.erase(word);
            }
        }
    }
}

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
    int length;
    cin >> length;
    setWordLength(length);

    while (true) {
        printGame();
        char compGuess = guess();
        
        // Tests to see if the hangman game is over.
        if (compGuess == '\0') {
            if (possibleWords.size() != 1) cout << "Sorry, I must not have your word in my dictionary." << endl;
            else if (wrongGuesses == 1) cout << "I got your word with only 1 wrong guess!" << endl;
            else cout << "I got your word with " << wrongGuesses << " wrong guesses!" << endl;
            char response;
            cout << "Would you like to start a new game? [Y/N] ";
            cin >> response;
            if (toupper(response) == 'Y') {
                cout << endl;
                playGame();
            }
            else break;
        }
        
        cout << "Is the letter '" << char(toupper(compGuess)) << "' in your word? [Y/N] ";
        char response;
        cin >> response;
        if (toupper(response) == 'Y') {
            cin.clear();
            cout << "Enter " << char(toupper(compGuess)) << "'s position(s) in the word, separated by commas. ";
            string line;
            vector<int> positions;
            cin.ignore();
            getline(std::cin, line);
            std::stringstream ss(line);
            int i;
            while (ss >> i)
            {
                positions.push_back(i);
                if (ss.peek() == ',' || ss.peek() == ' ') ss.ignore();
            }
            setGuessResult(compGuess, positions);
        }
        else setGuessResult(compGuess, {});
    }
}

// Generates sequence of guesses in order for any hangman word
vector<char> HangmanSolver::guesses(string word) {
    vector<char> guessSequence;
    setWordLength(int(word.length()));
    previousGuesses = {};
    wrongGuesses = 0;
    while (true) {
        char compGuess = guess();
        if (compGuess == '\0') break;
        guessSequence.push_back(compGuess);
        vector<int> positions;
        for (int i = 0; i < word.length(); i++) {
            if (word[i] == compGuess) positions.push_back(i + 1);
        }
        setGuessResult(compGuess, positions);
    }
    return guessSequence;
}