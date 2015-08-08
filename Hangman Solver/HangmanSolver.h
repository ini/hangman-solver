//
//  HangmanSolver.h
//  Hangman Solver
//
//  Created by Ini on 8/8/15.
//  Copyright (c) 2015 Insi Productions. All rights reserved.
//

#ifndef __Hangman_Solver__HangmanSolver__
#define __Hangman_Solver__HangmanSolver__

#include <stdio.h>
#include <set>
#include <map>
#include <string>
#include <vector>

using namespace std;

class HangmanSolver {
    public:
        HangmanSolver();
        void playGame();
        vector<char> guesses(string word);
    
    private:
        map<char, int> letterCounts();
        void setWordLength(int length);
        char guess();
        void setGuessResult(char guessedChar, vector<int> positions);
        void printGame();
        
        int wordLength;
        vector<char> wordChars;
        set<string> dictionary;
        set<string> possibleWords;
        set<char> previousGuesses;
        int wrongGuesses;
};

#endif /* defined(__Hangman_Solver__HangmanSolver__) */
