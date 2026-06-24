#include "ships.h"

static mt19937 GENERATOR(time(0));

int main()
{
    int boardSize = getBoardSize();
    size_t boardSizeSizeT = boardSize;

    vector<int> ships = shipSizes(boardSize);
    vector<string> playerBoard = fillWithSpaces(boardSizeSizeT);
    vector<string> enemyBoard = fillWithSpaces(boardSizeSizeT);

    playerBoard = createEnemyBoard(playerBoard, ships);
    enemyBoard = createEnemyBoard(enemyBoard, ships);

    playGame(playerBoard, enemyBoard);

    return EXIT_SUCCESS;
}

bool hasInputFailed()
{
    const bool hasFailed = cin.fail();

    if (hasFailed)
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "Blad typu!\n";
    }

    return hasFailed;
}

int getBoardSize()
{
    int boardSize = 0;

    while (boardSize <= 0)
    {
        cout << "Podaj wielkosc planszy: ";
        cin >> boardSize;

        if (!hasInputFailed())
        {
            boardSize = 0;
        }
    }

    return boardSize;
}

vector<int> generateShips(int boardSize)
{
    int maxShipSize = round(sqrt(boardSize * 1.6));
    vector<int> ships(maxShipSize + 1);

    for (int i = 1; i <= maxShipSize; i++)
    {
        ships[i] = maxShipSize + 1 - i;
    }

    return ships;
}

int currentOrientation()
{
    uniform_int_distribution orientation(0, 1);

    return orientation(GENERATOR);
}

bool canShipBePlaced(vector<string> &enemyBoard, int coordinatesY, int coordinatesX, int shipLength, int orientation)
{
    size_t boardSize = enemyBoard.size();

    if (coordinatesX + shipLength > boardSize)
    {
        return false;
    }

    if (orientation == 0)
    {
        for (int idxY = 0; idxY < shipLength; ++idxY)
        {
            if (enemyBoard[idxY + coordinatesY][coordinatesX] == 'S')
            {
                return false;
            }
        }

        return true;
    }

    for (int idxX = 0; idxX < shipLength; ++idxX)
    {
        if (enemyBoard[coordinatesY][coordinatesX + idxX] == 'S')
        {
            return false;
        }
    }

    return true;
}

vector<string> fillWithSpaces(size_t boardSize)
{
    vector<string> board(boardSize, string(boardSize, ' '));

    return board;
}

vector<string> createEnemyBoard(vector<string> &enemyBoard, vector<int> &ships)
{
    static mt19937 generator(time(0));
    int enemyBoardSize = enemyBoard.size();

    for (int shipLength = 1; shipLength < ships.size(); ++shipLength)
    {
        int shipQuantity = ships[shipLength];
        for (int currentShip = 1; currentShip <= shipQuantity; ++currentShip)
        {
            bool placed = false;
            int attempts = 0; // Zabezpieczenie przed nieskończonym losowaniem przy braku miejsca

            while (!placed && attempts < 100)
            {
                int orientation = currentOrientation();
                int coordinatesY = 0, coordinatesX = 0;

                if (orientation == 0) // Pionowo
                {
                    if (enemyBoardSize - shipLength >= 0)
                    {
                        uniform_int_distribution rangeY(0, enemyBoardSize - shipLength);
                        uniform_int_distribution rangeX(0, enemyBoardSize - 1);
                        coordinatesY = rangeY(generator);
                        coordinatesX = rangeX(generator);
                    }
                }
                else // Poziomo
                {
                    if (enemyBoardSize - shipLength >= 0)
                    {
                        uniform_int_distribution rangeX(0, enemyBoardSize - shipLength);
                        uniform_int_distribution rangeY(0, enemyBoardSize - 1);
                        coordinatesY = rangeY(generator);
                        coordinatesX = rangeX(generator);
                    }
                }

                if (canShipBePlaced(enemyBoard, coordinatesY, coordinatesX, shipLength, orientation))
                {
                    if (orientation == 0) // Pionowow
                    {
                        for (int idxY = 0; idxY < shipLength; ++idxY)
                        {
                            enemyBoard[idxY + coordinatesY][coordinatesX] = 'S';
                        }
                    }
                    else // Poziomo
                    {
                        for (int idxX = 0; idxX < shipLength; ++idxX)
                        {
                            enemyBoard[coordinatesY][coordinatesX + idxX] = 'S';
                        }
                    }
                    placed = true;
                }
                ++attempts;
            }
            // Jak sie cos spierpzylo iz le wygenerowalo, to jeszcze raz rekurnecja generujemy
            if (!placed)
            {
                enemyBoard = fillWithSpaces(enemyBoardSize);
                return createEnemyBoard(enemyBoard, ships);
            }
        }
    }
    return enemyBoard;
}

vector<pair<int, int>> getCoordinates(int boardSize)
{
    int y = 0, x = 0;
    while (true)
    {
        cout << "\nPodaj gdzie chcesz strzelic (np. 3 4), liczymy od 1: ";
        cin >> y >> x;

        if (!hasInputFailed())
            continue;

        if (y >= 1 && y <= boardSize && x >= 1 && x <= boardSize)
        {
            break;
        }
        cout << "Wspolrzedne poza plansza! Podaj liczby od 1 do " << boardSize << ".\n";
    }

    vector<pair<int, int>> coordinates(1);
    coordinates[0] = {y, x};
    return coordinates;
}

void changeEnemyBoard(vector<string> &enemyBoard)
{
    vector<pair<int, int>> coordinates = getCoordinates(enemyBoard.size());
    enemyBoard[coordinates[0].first - 1][coordinates[0].second - 1] = 'X';
}

void showBoard(vector<string> &board)
{
    size_t boardSize = board.size();

    for (size_t rowIdx = 0; rowIdx < boardSize; ++rowIdx)
    {
        cout << "|" << board[rowIdx] << "|\n";
    }
}

void changePlayerBoard(vector<string> &playerBoard)
{
    int playerBoardSizeInt = playerBoard.size();
    static mt19937 generator(time(0));
    uniform_int_distribution section(0, playerBoardSizeInt - 1);

    int y, x;
    y = section(generator);
    x = section(generator);

    while (playerBoard[y][x] == 'X')
    {
        y = section(generator);
        x = section(generator);
    }

    playerBoard[y][x] = 'X';
}

bool hasPlayerWon(vector<string> &board)
{
    size_t boardSize = board.size();

    for (size_t idxY = 0; idxY < boardSize; ++idxY)
    {
        for (size_t idxX = 0; idxX < boardSize; ++idxX)
        {
            if (board[idxY][idxX] == 'S')
                return true;
        }
    }

    return false;
}

void takeTurn(vector<string> &playerBoard, vector<string> &enemyBoard)
{
    cout << "--- TURA BOT-a (Strzela do Ciebie) ---\n";
    changePlayerBoard(playerBoard);
    showBoard(playerBoard);
    this_thread::sleep_for(chrono::seconds(5));
    system("cls");

    cout << "\n--- TWOJA TURA (Strzelasz do bota) ---\n";
    changeEnemyBoard(enemyBoard);
    showBoard(enemyBoard);
    this_thread::sleep_for(chrono::seconds(5));
    system("cls");
}

void playGame(vector<string> &playerBoard, vector<string> &enemyBoard)
{
    takeTurn(playerBoard, enemyBoard);
    while (hasPlayerWon(playerBoard) && hasPlayerWon(enemyBoard))
    {
        takeTurn(playerBoard, enemyBoard);
    }
    if (!hasPlayerWon(enemyBoard) && !hasPlayerWon(playerBoard))
        cout << "\nRemis!\n";
    else if (!hasPlayerWon(playerBoard))
        cout << "\nWygrales!\n";
    else
        cout << "\nPrzegrales!\n";
}