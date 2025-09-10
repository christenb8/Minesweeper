#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;
//
// Created by monic on 8/1/2025.
//

#ifndef TEXTURES_H
#define TEXTURES_H

struct TextureManager {
    map<string, sf::Texture> textureList;

    TextureManager() {
        sf::Texture tileHiddenTexture;
        if (!tileHiddenTexture.loadFromFile("../files/images/tile_hidden.png")) {
            cerr << "Could not load tileHiddenTexture" << endl;
        }
        sf::Texture tileRevealedTexture;
        if (!tileRevealedTexture.loadFromFile("../files/images/tile_revealed.png")) {
            cerr << "Could not load tileRevealedTexture" << endl;
        }
        sf::Texture happyFaceTexture;
        if (!happyFaceTexture.loadFromFile("../files/images/face_happy.png")) {
            cerr << "Could not load happyFaceTexture" << endl;
        }
        sf::Texture winFaceTexture;
        if (!winFaceTexture.loadFromFile("../files/images/face_win.png")) {
            cerr << "Could not load winFaceTexture" << endl;
        }
        sf::Texture loseFaceTexture;
        if (!loseFaceTexture.loadFromFile("../files/images/face_lose.png")) {
            cerr << "Could not load loseFaceTexture" << endl;
        }
        sf::Texture debugTexture;
        if (!debugTexture.loadFromFile("../files/images/debug.png")) {
            cerr << "Could not load debugTexture" << endl;
        }
        sf::Texture pauseTexture;
        if (!pauseTexture.loadFromFile("../files/images/pause.png")) {
            cerr << "Could not load pauseTexture" << endl;
        }
        sf::Texture playTexture;
        if (!playTexture.loadFromFile("../files/images/play.png")) {
            cerr << "Could not load playTexture" << endl;
        }
        sf::Texture leaderboardTexture;
        if (!leaderboardTexture.loadFromFile("../files/images/leaderboard.png")) {
            cerr << "Could not load leaderboardTexture" << endl;
        }
        sf::Texture mineTexture;
        if (!mineTexture.loadFromFile("../files/images/mine.png")) {
            cerr << "Could not load mineTexture" << endl;
        }
        sf::Texture oneTexture;
        if (!oneTexture.loadFromFile("../files/images/number_1.png")) {
            cerr << "Could not load oneTexture" << endl;
        }
        sf::Texture twoTexture;
        if (!twoTexture.loadFromFile("../files/images/number_2.png")) {
            cerr << "Could not load twoTexture" << endl;
        }
        sf::Texture threeTexture;
        if (!threeTexture.loadFromFile("../files/images/number_3.png")) {
            cerr << "Could not load threeTexture" << endl;
        }
        sf::Texture fourTexture;
        if (!fourTexture.loadFromFile("../files/images/number_4.png")) {
            cerr << "Could not load fourTexture" << endl;
        }
        sf::Texture fiveTexture;
        if (!fiveTexture.loadFromFile("../files/images/number_5.png")) {
            cerr << "Could not load fiveTexture" << endl;
        }
        sf::Texture sixTexture;
        if (!sixTexture.loadFromFile("../files/images/number_6.png")) {
            cerr << "Could not load sixTexture" << endl;
        }
        sf::Texture sevenTexture;
        if (!sevenTexture.loadFromFile("../files/images/number_7.png")) {
            cerr << "Could not load sevenTexture" << endl;
        }
        sf::Texture eightTexture;
        if (!eightTexture.loadFromFile("../files/images/number_8.png")) {
            cerr << "Could not load eightTexture" << endl;
        }
        sf::Texture flagTexture;
        if (!flagTexture.loadFromFile("../files/images/flag.png")) {
            cerr << "Could not load flagTexture" << endl;
        }
        sf::Texture digitsTexture;
        if (!digitsTexture.loadFromFile("../files/images/digits.png")) {
            cerr << "Could not load digitsTexture" << endl;
        }
        textureList.emplace("tileHiddenTexture", tileHiddenTexture);
        textureList.emplace("tileRevealedTexture", tileRevealedTexture);
        textureList.emplace("happyFaceTexture", happyFaceTexture);
        textureList.emplace("winFaceTexture", winFaceTexture);
        textureList.emplace("loseFaceTexture", loseFaceTexture);
        textureList.emplace("debugTexture", debugTexture);
        textureList.emplace("pauseTexture", pauseTexture);
        textureList.emplace("playTexture", playTexture);
        textureList.emplace("leaderboardTexture", leaderboardTexture);
        textureList.emplace("mineTexture", mineTexture);
        textureList.emplace("oneTexture", oneTexture);
        textureList.emplace("twoTexture", twoTexture);
        textureList.emplace("threeTexture", threeTexture);
        textureList.emplace("fourTexture", fourTexture);
        textureList.emplace("fiveTexture", fiveTexture);
        textureList.emplace("sixTexture", sixTexture);
        textureList.emplace("sevenTexture", sevenTexture);
        textureList.emplace("eightTexture", eightTexture);
        textureList.emplace("flagTexture", flagTexture);
        textureList.emplace("digitsTexture", digitsTexture);
    }
};

#endif //TEXTURES_H
