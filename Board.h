#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
#include "Textures.h"
#include "Tile.h"
using namespace std;

//
// Created by monic on 7/28/2025.
//

#ifndef BOARD_H
#define BOARD_H

class Board {
public:
    int width;
    int height;
    int rows;
    int cols;
    int mineCount;
    int counterCount;
    int tilesRevealed;
    vector<Tile> cells;
    bool flagsHidden;
    bool minesRevealed;
    int flagsPlaced;

    Board (int w, int h, int r, int c, int m) {
        width = w;
        height = h;
        rows = r;
        cols = c;
        mineCount = m;
        counterCount = m;
        tilesRevealed = 0;
        minesRevealed = false;
        flagsHidden = false;
        flagsPlaced = 0;
        cells.resize(rows * cols);
    }

    sf::VertexArray drawLines() {
        sf::VertexArray grid(sf::Lines, ((cols+1)* 2) + ((rows+1)*2));
        //draw columns
        float squareWidth = width/(cols*2);
        for (int i = 0; i < ((cols+1)*2); i+=2) {
            grid[i].position = {squareWidth * i, 0};
            grid[i+1].position = {squareWidth * i, (float)(height-100)};
            grid[i].color = sf::Color::Black;
            grid[i+1].color = sf::Color::Black;
        }
        //draw rows
        float squareHeight = (height-100)/(rows*2);
        for (int i = ((cols+1)* 2); i < ((rows+1)*2) + ((cols+1)* 2); i+=2) {
            grid[i].position = {0, squareHeight * (i- ((cols+1)* 2))};
            grid[i+1].position = {(float)width, squareHeight * (i- ((cols+1)* 2))};
            grid[i].color = sf::Color::Black;
            grid[i+1].color = sf::Color::Black;
        }

        return grid;
    }

    void drawTiles(TextureManager& t) {
        //Position and apply texture to all sprites
        float squareWidth = t.textureList["tileHiddenTexture"].getSize().x;
        float squareHeight = t.textureList["tileHiddenTexture"].getSize().y;
        for (int i = 0; i < cols; i++) {
            for (int j = 0; j < rows; j++) {
                cells[(cols * j) + i].sprite.setTexture(t.textureList["tileHiddenTexture"]);
                cells[(cols * j) + i].sprite.setPosition(i * squareWidth, j * squareHeight);
                cells[(cols * j) + i].overlay.setTexture(t.textureList["tileHiddenTexture"]);
                cells[(cols * j) + i].overlay.setPosition(i * squareWidth, j * squareHeight);
                cells[(cols * j) + i].flagSprite.setPosition(i * squareWidth, j * squareHeight);
                cells[(cols * j) + i].flagSprite.setTexture(t.textureList["flagTexture"]);
            }
        }

        //Set random tiles to mines
        srand(time(0));
        int minesAssigned = 0;
        while (minesAssigned < mineCount) {
            int randomIndex = (rand() % cells.size());
            if (!cells[randomIndex].isMine) {
                cells[randomIndex].isMine = true;
                minesAssigned += 1;
            }
        }
    }
    //Play
    void hideTiles(TextureManager& t) {
        for (int i = 0; i < cells.size(); i++) {
            if (!cells[i].isRevealed) {
                cells[i].sprite.setTexture(t.textureList["tileHiddenTexture"]);
                cells[i].overlay.setTexture(t.textureList["tileHiddenTexture"]);
            } else {
                displayNumbers(t, cells[i].adjacentMines, i);
            }
        }
        flagsHidden = false;

    }
    //Pause
    void revealTiles(TextureManager& t) {
        for (int i = 0; i < cells.size(); i++) {
            cells[i].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[i].overlay.setTexture(t.textureList["tileRevealedTexture"]);
            flagsHidden = true;
        }
    }

    void updateTiles(sf::RenderWindow& window) {
        for (int i = 0; i < cells.size(); i++) {
            window.draw(cells[i].sprite);

            if ((!minesRevealed) && (cells[i].hasFlag) && (!flagsHidden)) {
                window.draw(cells[i].overlay);
                window.draw(cells[i].flagSprite);
            } else if ((minesRevealed) && (cells[i].hasFlag) && (!flagsHidden)) {
                if (cells[i].isMine) {
                    window.draw(cells[i].flagSprite);
                    window.draw(cells[i].overlay);

                } else {
                    window.draw(cells[i].overlay);
                    window.draw(cells[i].flagSprite);
                }
            } else if ((minesRevealed) && (!cells[i].hasFlag)) {
                window.draw(cells[i].overlay);
            } else if ((!minesRevealed) && (!cells[i].hasFlag)) {
                window.draw(cells[i].overlay);
            }

        }
    }

    void debugMode(TextureManager& t) {
        for (int i = 0; i < cells.size(); i++) {
                if (cells[i].isMine) {
                    cells[i].sprite.setTexture(t.textureList["tileRevealedTexture"]);
                    cells[i].overlay.setTexture(t.textureList["mineTexture"]);
                }
        }
        minesRevealed = true;
    }

    void undoDebug(TextureManager& t) {
        for (int i = 0; i < cells.size(); i++) {
                if (!cells[i].isRevealed) {
                    cells[i].overlay.setTexture(t.textureList["tileHiddenTexture"]);
                }
                if (cells[i].hasFlag) {
                    cells[i].overlay.setTexture(t.textureList["tileHiddenTexture"]);
                }
        }
        minesRevealed = false;
    }

    void displayNumbers(TextureManager& t, int mines, int index) {
        if (mines == 1) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["oneTexture"]);
        } else if (mines == 2) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["twoTexture"]);
        } else if (mines == 3) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["threeTexture"]);
        } else if (mines == 4) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["fourTexture"]);
        } else if (mines == 5) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["fiveTexture"]);
        } else if (mines == 6) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["sixTexture"]);
        } else if (mines == 7) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["sevenTexture"]);
        } else if (mines == 8) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["eightTexture"]);
        }
    }

    void checkAdjacent(TextureManager& t, int index) {
        if ((cells[index].isRevealed) || (cells[index].isMine)) {
            return;
        }

        if (!cells[index].hasFlag) {
            cells[index].sprite.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].overlay.setTexture(t.textureList["tileRevealedTexture"]);
            cells[index].isRevealed = true;
            tilesRevealed += 1;
        }

        //check 8 tiles surrounding cells[index]
        int adjacentMines = 0;
        if (index % cols != 0) {
            if (cells[index - 1].isMine) {
                adjacentMines += 1;
            }
            if (index >= cols) {
                if (cells[index - 1 - cols].isMine) {
                    adjacentMines += 1;
                }
            }
            if ((index + cols) < (cols * rows)) {
                if (cells[index - 1 + cols].isMine) {
                    adjacentMines += 1;
                }
            }

        }
        if ((index+1) % cols != 0 ) {
            if (cells[index + 1].isMine) {
                adjacentMines += 1;
            }
            if (index >= cols) {
                if (cells[index + 1 - cols].isMine) {
                    adjacentMines += 1;
                }
            }
            if ((index + cols) < (cols * rows)) {
                if (cells[index + 1 + cols].isMine) {
                    adjacentMines += 1;
                }
            }
        }

        if (index >= cols) {
            if (cells[index - cols].isMine) {
                adjacentMines += 1;
            }
        }
        if ((index + cols) < (cols * rows)) {
            if (cells[index + cols].isMine) {
                adjacentMines += 1;
            }
        }

        cells[index].adjacentMines = adjacentMines;

        //START REVEALING TILES
        if (adjacentMines == 0) {
            if (index % cols != 0) {
                if (!cells[index - 1].isRevealed) {
                    checkAdjacent(t, index - 1);
                }
                if (index >= cols && !cells[index - 1 - cols].isRevealed) {
                    checkAdjacent(t, index - 1 - cols);
                }
                if (index + cols < cols * rows && !cells[index - 1 + cols].isRevealed) {
                    checkAdjacent(t, index - 1 + cols);
                }
            }

            if ((index + 1) % cols != 0) {
                if (!cells[index + 1].isRevealed) {
                    checkAdjacent(t, index + 1);
                }
                if (index >= cols && !cells[index + 1 - cols].isRevealed) {
                    checkAdjacent(t, index + 1 - cols);
                }
                if (index + cols < cols * rows && !cells[index + 1 + cols].isRevealed) {
                    checkAdjacent(t, index + 1 + cols);
                }
            }

            if (index >= cols && !cells[index - cols].isRevealed) {
                checkAdjacent(t, index - cols);
            }
            if (index + cols < cols * rows && !cells[index + cols].isRevealed) {
                checkAdjacent(t, index + cols);
            }
        } else {
            if (!cells[index].hasFlag)
                displayNumbers(t, adjacentMines, index);
        }
    }

    bool checkWin() {
        if (tilesRevealed == ((rows * cols) - mineCount)) {
            return true;
        }
        int count = 0;
        for (int i = 0; i < cells.size(); i++) {
            if (cells[i].hasFlag && cells[i].isMine) {
                count += 1;
            }
        }
        if (count == mineCount) {
            return true;
        }
        return false;
    }

    void placeFlag(TextureManager& t, int index) {
        if ((!cells[index].hasFlag) && (!cells[index].isRevealed)){
            cells[index].hasFlag = true;
            counterCount -= 1;

        } else if (cells[index].hasFlag) {
            cells[index].hasFlag = false;
            counterCount += 1;
        }
    }

    void resetTiles() {
        for (int i = 0; i < cells.size(); i++) {
            cells[i].isMine = false;
            cells[i].hasFlag = false;
            cells[i].isRevealed = false;
        }
        tilesRevealed = 0;
        flagsPlaced = 0;
        counterCount = mineCount;
        flagsHidden = false;
        minesRevealed = false;
    }

    void openLeaderboard(TextureManager& t, sf::RenderWindow& leaderboardWindow, sf::RenderWindow& gameWindow, sf::Text& leaderboardTitle, sf::Text& leaderboardText, int index, bool& leaderboardOpen, bool& gamePaused) {
        //Leaderboard Window ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        revealTiles(t);
        updateTiles(gameWindow);
        gameWindow.display();
        while ((leaderboardWindow.isOpen())) {
            sf::Event eventLb;
            while(leaderboardWindow.pollEvent(eventLb)) {
                if(eventLb.type == sf::Event::Closed) {
                    leaderboardWindow.setVisible(false);
                    leaderboardOpen = false;
                    index = 0;
                    leaderboardWindow.close();
                    if (!gamePaused) {
                        hideTiles(t);
                    }
                }
                leaderboardWindow.clear(sf::Color::Blue);
                leaderboardWindow.draw(leaderboardTitle);
                leaderboardWindow.draw(leaderboardText);
                leaderboardWindow.display();
            }
            sf::Event event;
            while (gameWindow.pollEvent(event)) {

            }
        }
    }

    void counterUpdate(TextureManager& t, sf::Sprite& counter0, sf::Sprite& counter1, sf::Sprite& counter2, sf::Sprite& counter3, bool gameWon) {
        int first = (counterCount - (counterCount % 100)) / 100;
        int middle = (counterCount - (counterCount % 10) ) / 10;
        if (counterCount > 99) {
            middle = middle - (10 * (counterCount / 100));
        }
        int last = counterCount % 10;

        if (counterCount < 0) {
            if (counterCount < -99) {
                first = abs(first);
                middle = abs(middle - (10 * (counterCount / 100)));
                last = abs(last);
                counter0.setPosition(12, ((rows + 0.5) * 32) + 16);
                counter0.setTextureRect(sf::IntRect(9 * 21, 0, 21, 32));
            } else {
                first = abs(first);
                middle = abs(middle);
                last = abs(last);
                counter0.setPosition(12, ((rows + 0.5) * 32) + 16);
                counter0.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
            }
        } else if (counterCount > 0){
            //counter0.setPosition(-12,-32);
            //counter0.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        }

         if (gameWon) {
            first = 0;
            middle = 0;
            last = 0;
        }

        //counter0.setPosition(12, ((rows + 0.5) * 32) + 16);
        //counter0.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        counter1.setPosition(33, ((rows + 0.5) * 32) + 16);
        counter1.setTextureRect(sf::IntRect(first * 21, 0, 21, 32));
        counter2.setPosition(33 + (1 * 21), ((rows + 0.5) * 32) + 16);
        counter2.setTextureRect(sf::IntRect(middle * 21, 0, 21, 32));
        counter3.setPosition(33 + (2 * 21), ((rows + 0.5) * 32) + 16);
        counter3.setTextureRect(sf::IntRect(last * 21, 0, 21, 32));

    }

    void updateTime(TextureManager& t, sf::Sprite& timer1, sf::Sprite& timer2, sf::Sprite& timer3, sf::Sprite& timer4, long long time, int clicks) {
        int firstInt = 0;
        int secondInt = 0;
        int thirdInt = 0;
        int fourthInt = 0;

        firstInt = (time/60) / 10;
        secondInt = (time/60) % 10;
        thirdInt = (time % 60) / 10;
        fourthInt = (time % 60) % 10;


        if (clicks == 0) {
            firstInt = 0;
            secondInt = 0;
            thirdInt = 0;
            fourthInt = 0;
        }

        //third digit
        timer1.setPosition(((cols * 32) - 97) + (0 * 21), ((rows + 0.5) * 32) + 16);
        timer1.setTextureRect(sf::IntRect(firstInt * 21, 0, 21, 32));
        //last digit
        timer2.setPosition(((cols * 32) - 97) + (1 * 21), ((rows + 0.5) * 32) + 16);
        timer2.setTextureRect(sf::IntRect(secondInt * 21, 0, 21, 32));
        timer3.setPosition(((cols * 32) - 54) + (0 * 21), ((rows + 0.5) * 32) + 16);
        timer3.setTextureRect(sf::IntRect(thirdInt * 21, 0, 21, 32));
        timer4.setPosition(((cols * 32) - 54) + (1 * 21), ((rows + 0.5) * 32) + 16);
        timer4.setTextureRect(sf::IntRect(fourthInt * 21, 0, 21, 32));
    }

    void writeToLeaderboard(vector<int>& times, vector<string> data, long long time, string username, int& index) {

        int firstInt = (time/60) / 10;
        int secondInt = (time/60) % 10;
        int thirdInt = (time % 60) / 10;
        int fourthInt = (time % 60) % 10;

        int timeTaken = (firstInt * 1000) + (secondInt * 100) + (thirdInt * 10) + (fourthInt * 1);
        ofstream file("../files/leaderboard.txt");
        if (!file.is_open()) {
            cerr << "Leaderboard file not opened." << endl;
        }
        // 07:01,Alex
        // 09:04,Bruce
        // 10:03,Carl
        // 11:24,Daniel
        // 12:15,Eve

        for (int i = 0; i < times.size(); i++) {
            if (timeTaken <= times[i]) {
                index = i;
                times.insert(times.begin() + i, timeTaken);

                string toInsert = to_string(firstInt) + to_string(secondInt) + ":" + to_string(thirdInt) + to_string(fourthInt) + "," + username;
                data.insert(data.begin() + i, toInsert);
                break;
            }
        }

        for (int i = 0; i < 5; i++) {
            file << data[i] << endl;
        }
    }

    void placeWinFlags(TextureManager& t) {
        for (int i = 0; i < cells.size(); i++) {
            if (!cells[i].hasFlag && cells[i].isMine) {
                cells[i].hasFlag = true;
            }
        }
    }

};

#endif //BOARD_H
