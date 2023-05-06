#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>


bool isCharInVector(char c, std::vector<char> guessedLetters) {
    return std::find(guessedLetters.begin(), guessedLetters.end(), c) != guessedLetters.end();
}

std::string giveRandomWord() {
    std::string words[5] = {"hangman", "apple", "banana", "orange", "pear"};
    srand(time(NULL));  // seed the random number generator with the current time
    return words[rand() % 5];
}

std::string hideWord(std::string word, std::vector<char> guessedLetters) {
    std::string hiddenWord = word;
    for (int i = 0; i < word.length(); i++) {
        if (!isCharInVector(word[i], guessedLetters)) {
            hiddenWord[i] = '_';
        }
    }
    return hiddenWord;
}

void processGuess(
    const std::string& guess, 
    const std::string wordToGuess, 
    const std::vector<char>& lettersInWordToGuess, 
    std::vector<char>& guessedLetters, 
    int& lives,
    bool& gameLost
) {
    if (guess.length() == 1) {
        guessedLetters.push_back(guess[0]);

        if (!isCharInVector(guess[0], lettersInWordToGuess)) {
            std::cout << "Wrong guess!" << std::endl;
            lives--;
            return;
        }

        std::cout << "Correct guess!" << std::endl;
        return;
    }

    if (guess == wordToGuess) {
        lives = 0;
        return;
    }

    std::cout << "Wrong guess!" << std::endl;
    lives--;
    if(lives == 0) gameLost = true;
}

void displayHangman(std::string wordToGuess, std::vector<char> guessedLetters, int lives) {
    bool gameLost = false;
    while(lives > 0) {
        std::vector<char> lettersInWordToGuess = std::vector<char>(wordToGuess.begin(), wordToGuess.end());

        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) {
            if(isCharInVector(c, lettersInWordToGuess)) continue;
            std::cout << c << " ";
        }
        std::cout << "\n";
        std::cout << hideWord(wordToGuess, guessedLetters) << std::endl;

        std::string guess;
        std::cin >> guess;

        processGuess(
            guess, 
            wordToGuess, 
            lettersInWordToGuess, 
            guessedLetters, 
            lives, 
            gameLost
        );
    }

    std::cout << "You " << (gameLost ? "lost" : "won") << "! The word was: " << wordToGuess << std::endl;
}

int main() {
    const std::string wordToGuess = giveRandomWord();
    std::vector<char> guessedLetters;
    int lives = 7;
    
    displayHangman(wordToGuess, guessedLetters, lives);

    system("pause >nul");
    return 0;
}