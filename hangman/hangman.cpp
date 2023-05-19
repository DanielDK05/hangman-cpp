#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <fstream>

const int STARTING_LIVES = 6;

bool isCharInVector(char c, std::vector<char> guessedLetters) {
    return std::find(guessedLetters.begin(), guessedLetters.end(), c) != guessedLetters.end();
}

std::string giveRandomWord() {
    std::ifstream file("words.txt");

    if (!file.is_open()) {
        std::cerr << "Failed to open file" << std::endl;
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    std::vector<std::string> lines;
    std::string line;
    std::istringstream iss(content);

    while (std::getline(iss, line)) {
        lines.push_back(line);
    }

    srand(time(NULL));
    return lines[rand() % lines.size()];
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

std::vector<std::string> getAsciiFiles() {
    std::vector<std::string> files;
    std::string path = std::filesystem::current_path().string() + "/ascii/";

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            if (file) {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                files.push_back(content);
            }
            else {
                std::cerr << "Error opening file " << entry.path() << std::endl;
            }
        }
    }

    return files;
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
            lives--;
            return;
        }

        return;
    }

    if (guess == wordToGuess) {
        lives = 0;
        return;
    }

    lives--;
    if (lives == 0) gameLost = true;
}

void displayHangman(std::string wordToGuess, std::vector<char> guessedLetters, int lives) {
    bool gameLost = false;
    std::string guess;

    while (lives >= 0) {
        std::system("CLS");

        std::vector<char> lettersInWordToGuess = std::vector<char>(wordToGuess.begin(), wordToGuess.end());

        std::cout << getAsciiFiles()[static_cast<size_t>(STARTING_LIVES) - static_cast<size_t>(lives)] << std::endl;

        std::cout << "Lives left: " << lives << std::endl;
        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) {
            if (isCharInVector(c, lettersInWordToGuess)) continue;
            std::cout << c << " ";
        }
        std::cout << "\n";
        std::cout << hideWord(wordToGuess, guessedLetters) << std::endl;
        
        if (lives == 0) break;

        std::getline(std::cin, guess); // Read the entire line as the guess

        if (!guess.empty()) {
            processGuess(
                guess,
                wordToGuess,
                lettersInWordToGuess,
                guessedLetters,
                lives,
                gameLost
            );
        }
    }

    std::cout << "You " << (gameLost ? "lost" : "won") << "! The word was: " << wordToGuess << std::endl;
}

int main() {
    bool continueGame = true;
    
    while (continueGame == true) {
        const std::string wordToGuess = giveRandomWord();
        std::vector<char> guessedLetters;
        int lives = STARTING_LIVES;

        displayHangman(wordToGuess, guessedLetters, lives);

        std::string s;
        std::cout << "Play again?" << std::endl;
        std::cin >> s;

        if (s != "yes" && s != "Yes" && s != "y" && s != "Y") continueGame = false;
    }

    return 0;
}