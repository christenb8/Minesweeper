#include <chrono>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>

#include "Board.h"
using namespace std;

void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool buttonClicked(sf::Sprite button, int x, int y) {
    if ((button.getGlobalBounds().left <= x) && (x <= button.getGlobalBounds().left + button.getLocalBounds().width) && (button.getGlobalBounds().top <= y) && (y <= button.getGlobalBounds().top + button.getLocalBounds().height)) {
        return true;
    }
    return false;
}


int main() {

    ifstream file("../files/config.cfg");
    if (!file.is_open()) {
        return 1;
    }
    string colInput = "";
    getline(file, colInput);
    string rowInput = "";
    getline(file, rowInput);
    string mineInput = "";
    getline(file, mineInput);

    int colCount = stoi(colInput);
    int rowCount = stoi(rowInput);
    int mineCount = stoi(mineInput);

    sf::RenderWindow welcomeWindow(sf::VideoMode((colCount * 32),(rowCount * 32) + 100), "Welcome window", sf::Style::Close);
    int height = (rowCount * 32) + 100;
    int width = colCount * 32;

    TextureManager t;

    sf::Font font;
    if (!font.loadFromFile("../files/font.ttf")) {
        return -1;
    }
    sf::Text welcometext;
    welcometext.setFont(font);
    welcometext.setString("WELCOME TO MINESWEEPER!");
    welcometext.setCharacterSize(24);
    welcometext.setColor(sf::Color::White);
    welcometext.setStyle(sf::Text::Bold);
    welcometext.setStyle(sf::Text::Underlined);
    setText(welcometext, (width/2), (height/2) - 150);

    sf::Text nametext;
    nametext.setFont(font);
    nametext.setString("Enter your name:");
    nametext.setColor(sf::Color::White);
    nametext.setStyle(sf::Text::Bold);
    nametext.setCharacterSize(20);
    setText(nametext, width/2, (height/2) - 75);

    sf::Text inputText;
    inputText.setFont(font);
    inputText.setString("|");
    inputText.setColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    inputText.setCharacterSize(18);
    setText(inputText, width/2, (height/2) - 45);
    string username = "";

    bool openGameWindow = false;

    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
            }
            if (event.type == sf::Event::TextEntered) {
                if ((event.text.unicode == 13) && (username.length() > 0)){
                    openGameWindow = true;
                    welcomeWindow.close();
                }
                if ((event.text.unicode == '\b') && (username.length() != 0)) {
                    username.erase(username.length() -1);
                }
                if ((username.length() < 10) && ((event.text.unicode >= 'a' && event.text.unicode <= 'z') || (event.text.unicode >= 'A' && event.text.unicode <= 'Z'))){
                    if (username.length() == 0) {
                        username += toupper(event.text.unicode);
                    } else {
                        username += tolower(event.text.unicode);
                    }
                }
                inputText.setString(username + "|");
                setText(inputText, width/2, (height/2) - 45);
            }
        }

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcometext);
        welcomeWindow.draw(nametext);
        welcomeWindow.draw(inputText);
        welcomeWindow.display();
    }

    //END OF WELCOME WINDOW



    //START GAME WINDOW
    bool gamePaused = false;
    bool debugOn = false;
    bool gameOver = false;
    bool gameWon = false;
    bool leaderboardOpen = false;
    long long timeElapsed = 0;
    long long pauseTime = 0;
    chrono::system_clock::time_point timePaused;
    int indexAddedToLeaderboard = -1;
    int index = 0;

    sf::RenderWindow gameWindow(sf::VideoMode((colCount * 32),(rowCount * 32) + 100), "Minesweeper", sf::Style::Close);
    Board board(width, height, rowCount, colCount, mineCount);
    sf::VertexArray grid = board.drawLines();
    board.drawTiles(t);

    sf::Sprite happyFace(t.textureList["happyFaceTexture"]);
    sf::Sprite debug(t.textureList["debugTexture"]);
    sf::Sprite pause(t.textureList["pauseTexture"]);
    sf::Sprite leaderboard(t.textureList["leaderboardTexture"]);
    sf::Sprite counter0(t.textureList["digitsTexture"]);
    sf::Sprite counter1(t.textureList["digitsTexture"]);
    sf::Sprite counter2(t.textureList["digitsTexture"]);
    sf::Sprite counter3(t.textureList["digitsTexture"]);
    sf::Sprite timer1(t.textureList["digitsTexture"]);
    sf::Sprite timer2(t.textureList["digitsTexture"]);
    sf::Sprite timer3(t.textureList["digitsTexture"]);
    sf::Sprite timer4(t.textureList["digitsTexture"]);



    happyFace.setPosition((colCount/2.0) * 32, (rowCount + 0.5) * 32);
    debug.setPosition((colCount * 32) - 304, (rowCount + 0.5) * 32);
    pause.setPosition((colCount * 32) - 240, (rowCount + 0.5) * 32);

    board.counterUpdate(t, counter0, counter1, counter2, counter3, gameWon);

    leaderboard.setPosition((colCount * 32) - 176, (rowCount + 0.5) * 32);

    int clicks = 0;
    auto beginTime = chrono::high_resolution_clock::now();

    while (gameWindow.isOpen() && openGameWindow) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                gameWindow.close();
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                //ALL CLICKS ON THE GRID
                if (clicks == 0) {
                    beginTime = chrono::high_resolution_clock::now();
                }
                clicks += 1;
                if ((y <= (rowCount * 32)) && !gameOver && !leaderboardOpen && !gamePaused) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        for (int i = 0; i < board.cells.size(); i++) {
                            if ((buttonClicked(board.cells[i].sprite, x, y)) && !board.cells[i].hasFlag) {
                                if (board.cells[i].isMine) {
                                    board.debugMode(t);
                                    gameOver = true;
                                    gameWon = false;
                                } else {
                                    board.checkAdjacent(t, i);
                                }
                                break;
                            }
                        }
                    } else if (event.mouseButton.button == sf::Mouse::Right) {
                        for (int i = 0; i < board.cells.size(); i++) {
                            if ((buttonClicked(board.cells[i].sprite, x, y)) && !debugOn) {
                                board.placeFlag(t, i);
                                board.counterUpdate(t, counter0, counter1, counter2, counter3, gameWon);
                                break;
                            }
                        }
                    }

                }

                //CLICKS FOR ALL BUTTONS
                //PAUSE BUTTON
                if ((buttonClicked(pause, x, y)) && !gameOver && !leaderboardOpen) {
                    if (gamePaused) {
                        gamePaused = false;
                        pause.setTexture(t.textureList["pauseTexture"]);
                        board.hideTiles(t);
                        pauseTime += (chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - timePaused)).count();
                        if (debugOn) {
                            board.debugMode(t);
                        }
                    } else {
                        timePaused = chrono::high_resolution_clock::now();
                        gamePaused = true;
                        pause.setTexture(t.textureList["playTexture"]);
                        board.revealTiles(t);
                    }
                }
                if ((buttonClicked(debug, x, y)) && !gameOver && !leaderboardOpen && !gamePaused) {
                    if (!debugOn) {
                        board.debugMode(t);
                        debugOn = true;
                    } else {
                        board.undoDebug(t);
                        debugOn = false;
                    }
                }
                if ((buttonClicked(happyFace, x, y)) && gameOver && !leaderboardOpen) {
                    // gamePaused = false;
                    // debugOn = false;
                    // gameOver = false;
                    // gameWon = false;
                    index = 0;
                    clicks = 0;
                    gamePaused = false;
                    debugOn = false;
                    gameOver = false;
                    gameWon = false;
                    leaderboardOpen = false;
                    timeElapsed = 0;
                    pauseTime = 0;
                    chrono::system_clock::time_point timePaused;
                    indexAddedToLeaderboard = -1;
                    index = 0;
                    board.resetTiles();
                    board.drawTiles(t);
                    board.counterUpdate(t, counter0, counter1, counter2, counter3, gameWon);
                    happyFace.setTexture(t.textureList["happyFaceTexture"]);
                }
                if ((buttonClicked(leaderboard, x, y)) && !leaderboardOpen) {
                    int index = 0;
                    sf::RenderWindow leaderboardWindow(sf::VideoMode((colCount * 16),(rowCount * 16) + 50), "Leaderboard", sf::Style::Close);

                    sf::Text leaderboardTitle;
                    leaderboardTitle.setFont(font);
                    leaderboardTitle.setString("LEADERBOARD");
                    leaderboardTitle.setCharacterSize(20);
                    leaderboardTitle.setColor(sf::Color::White);
                    leaderboardTitle.setStyle(sf::Text::Bold);
                    leaderboardTitle.setStyle(sf::Text::Underlined);
                    setText(leaderboardTitle, ((colCount * 16)/2), (((rowCount * 16) + 50)/2) - 120);

                    sf::Text leaderboardText;
                    leaderboardText.setFont(font);
                    leaderboardText.setCharacterSize(18);
                    leaderboardText.setColor(sf::Color::White);
                    leaderboardText.setStyle(sf::Text::Bold);

                    ifstream fileL("../files/leaderboard.txt");
                    if (!fileL.is_open()) {
                        return 1;
                    }
                    string leaderboardInput = "";
                    vector<int> leaderboardTimes;
                    for (int i = 0; i < 5; i++) {
                        string line = "";
                        getline(fileL, line);
                        if (i != 4) {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6) + "\n\n";
                        } else {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6);
                        }
                        leaderboardTimes.push_back(stoi(line.substr(0,1)) * 1000 + stoi(line.substr(1,1)) * 100 + stoi(line.substr(3,1)) * 10 + stoi(line.substr(4,1)));
                    }
                    leaderboardText.setString(leaderboardInput);

                    setText(leaderboardText, ((colCount * 16)/2), (((rowCount * 16) + 50)/2) + 20);
                    if (index == 0) {
                        board.openLeaderboard(t, leaderboardWindow, gameWindow, leaderboardTitle, leaderboardText, index, leaderboardOpen, gamePaused);
                        if (!(gamePaused) && (debugOn) || ((gameOver) && !gameWon)){
                            board.debugMode(t);
                        }
                    }
                }
            }
        }

        auto endTime = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::seconds>(endTime - beginTime);

        if ((!gameOver) && (!gamePaused)) {
            board.updateTime(t, timer1, timer2, timer3, timer4, duration.count() - pauseTime, clicks);
        }

        if (board.checkWin()) {
            gameWon = true;
            gameOver = true;
            happyFace.setTexture(t.textureList["winFaceTexture"]);

            board.placeWinFlags(t);
            board.counterCount = 0;
            if (index == 0)
                board.counterUpdate(t, counter0, counter1, counter2, counter3, gameWon);

            if (board.counterCount < 0) {
                gameWindow.draw(counter0);
            }
            board.undoDebug(t);
            board.updateTiles(gameWindow);

            gameWindow.draw(counter1);
            gameWindow.draw(counter2);
            gameWindow.draw(counter3);
            gameWindow.draw(happyFace);
            gameWindow.display();
        }

        gameWindow.clear(sf::Color::White);
        gameWindow.draw(grid);
        board.updateTiles(gameWindow);
        gameWindow.draw(happyFace);
        gameWindow.draw(debug);
        gameWindow.draw(pause);
        gameWindow.draw(leaderboard);
        if (board.counterCount < 0) {
            gameWindow.draw(counter0);
        }
        gameWindow.draw(counter1);
        gameWindow.draw(counter2);
        gameWindow.draw(counter3);
        gameWindow.draw(timer1);
        gameWindow.draw(timer2);
        gameWindow.draw(timer3);
        gameWindow.draw(timer4);
        gameWindow.display();

        if (gameOver) {
            if ((gameWon) && (index == 0)) {
                sf::RenderWindow leaderboardWindow(sf::VideoMode((colCount * 16),(rowCount * 16) + 50), "Leaderboard", sf::Style::Close);
                sf::Text leaderboardTitle;
                leaderboardTitle.setFont(font);
                leaderboardTitle.setString("LEADERBOARD");
                leaderboardTitle.setCharacterSize(20);
                leaderboardTitle.setColor(sf::Color::White);
                leaderboardTitle.setStyle(sf::Text::Bold);
                leaderboardTitle.setStyle(sf::Text::Underlined);
                setText(leaderboardTitle, ((colCount * 16)/2), (((rowCount * 16) + 50)/2) - 120);

                sf::Text leaderboardText;
                leaderboardText.setFont(font);
                leaderboardText.setCharacterSize(18);
                leaderboardText.setColor(sf::Color::White);
                leaderboardText.setStyle(sf::Text::Bold);

                ifstream fileL("../files/leaderboard.txt");
                if (!fileL.is_open()) {
                    return 1;
                }

                string leaderboardInput = "";
                vector<int> leaderboardTimes;
                vector<string> leaderboardData;

                for (int i = 0; i < 5; i++) {
                    string line = "";
                    getline(fileL, line);
                    leaderboardData.push_back(line);
                    leaderboardTimes.push_back(stoi(line.substr(0,1)) * 1000 + stoi(line.substr(1,1)) * 100 + stoi(line.substr(3,1)) * 10 + stoi(line.substr(4,1)));
                }
                auto gameEndTime = chrono::high_resolution_clock::now();
                auto durationEnd = chrono::duration_cast<chrono::seconds>(gameEndTime - beginTime);
                board.writeToLeaderboard(leaderboardTimes, leaderboardData, durationEnd.count(), username, indexAddedToLeaderboard);
                fileL.close();


                ifstream fileL2("../files/leaderboard.txt");
                if (!fileL2.is_open()) {
                    return 1;
                }
                for (int i = 0; i < 5; i++) {
                    string line = "";
                    getline(fileL2, line);
                    leaderboardData.push_back(line);
                    leaderboardTimes.push_back(stoi(line.substr(0,1)) * 1000 + stoi(line.substr(1,1)) * 100 + stoi(line.substr(3,1)) * 10 + stoi(line.substr(4,1)));
                    if (i != 4) {
                        if (i == indexAddedToLeaderboard) {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6) + "*" + "\n\n";
                        } else {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6) + "\n\n";
                        }
                    } else {
                        if (i == indexAddedToLeaderboard) {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6) + "*";
                        } else {
                            leaderboardInput += to_string(i + 1) + "." + "\t" + line.substr(0,5) + "\t" + line.substr(6);
                        }
                    }
                }

                leaderboardText.setString(leaderboardInput);
                setText(leaderboardText, ((colCount * 16)/2), (((rowCount * 16) + 50)/2) + 20);

                leaderboardOpen = true;
                if (index == 0) {
                    board.openLeaderboard(t, leaderboardWindow, gameWindow, leaderboardTitle, leaderboardText, index, leaderboardOpen, gamePaused);
                    if (debugOn && !gamePaused) {
                        board.debugMode(t);
                    }
                    index += 1;
                }
            }
            else if (!gameWon){
                happyFace.setTexture(t.textureList["loseFaceTexture"]);
            }
        }

    }
    return 0;
}