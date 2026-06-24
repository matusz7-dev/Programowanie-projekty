#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <random>
#include <ctime>
#include <cmath>

using namespace std;

namespace ships
{
    bool hasInputFailed() {}
    int getBoardSize() {}
    vector<int> shipSizes(int boardSize) {}
    int curOrientaiton() {}
    bool canShipBePlaced(vector<string> &enemyBoard, int coordinatesY, int coordinatesX, int shipLength, int orientation) {}
    vector<string> fillWithSpaces(size_t boardSize) {}
    vector<string> createEnemyBoard(vector<string> &enemyBoard, vector<int> &ships) {}
    vector<pair<int, int>> getCoordinates(int boardSize) {}
    void changeEnemyBoard(vector<string> &enemyBoard) {}
    void showBoard(vector<string> &enemyBoard) {}
    void changePlayerBoard(vector<string> &playerBoard) {}
    bool isWin(vector<string> &board) {}
    void playTurn(vector<string> &playerBoard, vector<string> &enemyBoard) {}
    void playGame(vector<string> &playerBoard, vector<string> &enemyBoard) {}
}