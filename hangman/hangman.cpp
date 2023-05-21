#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <conio.h> // for _getch and _kbhit
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

/*
return 0 for wrong guess
return 1 for correct character guess
return 2 for correct word guess (win)
return 3 for already guessed letter
*/
int processGuess(
    const std::string guess,
    const std::string wordToGuess,
    const std::vector<char> lettersInWordToGuess,
    std::vector<char>& guessedLetters
) {
    if (guess.length() == 1) {
        if (isCharInVector(guess[0], guessedLetters)) return 3;

        guessedLetters.push_back(guess[0]);

        return isCharInVector(guess[0], lettersInWordToGuess);
    }

    return guess == wordToGuess ? 2 : 0;
}

void displayHangman(std::string wordToGuess, std::vector<char> guessedLetters) {
    bool gameWon = false;
    int lives = STARTING_LIVES;
    std::string guessResultMessage;
    std::string guess;
    std::vector<std::string> asciiImages = getAsciiFiles();

    while (lives > 0) {
        std::system("CLS");

        std::vector<char> lettersInWordToGuess(wordToGuess.begin(), wordToGuess.end());

        std::cout << asciiImages[static_cast<size_t>(STARTING_LIVES) - static_cast<size_t>(lives)] << std::endl;

        std::cout << "Lives left: " << lives << std::endl;
        std::cout << "Guessed letters: ";
        for (char c : guessedLetters) {
            if (isCharInVector(c, lettersInWordToGuess)) continue;
            std::cout << c << " ";
        }
        std::cout << "\n";
        std::cout << hideWord(wordToGuess, guessedLetters) << std::endl;
        if(!guessResultMessage.empty()) std::cout << guessResultMessage << std::endl;

        std::getline(std::cin, guess); // Read the entire line as the guess

        if (guess.empty()) continue;

        int guessResult = processGuess(
            guess,
            wordToGuess,
            lettersInWordToGuess,
            guessedLetters
        );

        if (guessResult == 3) {
            guessResultMessage = "You have already guessed that!";
        } else if (guessResult == 2) {
            gameWon = true;
            break;
        }
        else if (guessResult == 1) {
            guessResultMessage = "You guessed correctly!";
        }
        else {
            lives--;
            guessResultMessage = "You guessed incorrectly!";
            if (lives == 0) {
                break;
            }
        }
    }

    std::system("CLS");
    std::cout << "You " << (gameWon ? "won" : "lost") << "! The word was: " << wordToGuess << std::endl;
    std::system("pause");
}

bool askUserToContinue() {
    bool userWantsContinue = false;

    while (true) {
        std::system("CLS");
        std::cout << "Do you want to play another time?" << std::endl;

        if (userWantsContinue) {
            std::cout << "<Yes> :  No " << std::endl;
        }
        else {
            std::cout << " Yes  : <No>" << std::endl;
        }

        while (true)
        {
            if (_kbhit()) {
                int key = _getch();

                if (key == 224) {
                    int arrowKey = _getch();

                    if (arrowKey == 75) {
                        userWantsContinue = true;
                        break;
                    }
                    else if (arrowKey == 77) {
                        userWantsContinue = false;
                        break;
                    }
                }
                else if (key == 13) { // Enter key
                    return userWantsContinue;
                }
            }
        }
    }
}

int main() {
    bool continueGame = true;
    
    while (continueGame == true) {
        const std::string wordToGuess = giveRandomWord();
        std::vector<char> guessedLetters;

        displayHangman(wordToGuess, guessedLetters);

        continueGame = askUserToContinue();
    }

    return 0;
}