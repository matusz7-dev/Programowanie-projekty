// Kolko i krzyzyk.cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int gameBeggining()
{
    int boardSize;
    cout << "Kolko i krzyzyk wygrywa ten gracz, ktory ma tyle znakow w tej samej linii co wielkosc planszy" << "\n" << "\n";
    cout << "Podaj wielkosc planszy" << "\n";
    cin >> boardSize;
    if (boardSize <= 0)
    {
        cout << "Nie mozesz dawac ujemnych i 0 wielkosci" << "\n";
        gameBeggining();
    }
    else
    {
        return boardSize;
    }
}

vector<vector<string>>board(int size)
{
    vector<vector<string>>vektorIdx(size);
    int row = 0;
    for (auto i = 1;i <= size * size;++i)
    {
        vektorIdx[row].push_back(to_string(i));
        if (i % size == 0)
        {
            ++row;
        }
    }
    return (vektorIdx);
}

bool isOx(int number, int maxSize)
{
    if (number == 0)
        return false;
    if (number == maxSize)
        return false;
    else if (number % 2 == 0)
        return true;
    else
        return false;
}

bool isOy(int column, int digits,int size)
{
    if (column == 1)
        return false;
    else if (column == size)
        return false;
    else if (column % (digits+1) == 0)
        return true;
    return false;
}

int howManyDigits(int number)
{
    int digits = 0;
    while (number > 0)
    {
        number /= 10;
        ++digits;
    }
    return digits;
}

bool isDigit(string element)
{
    if (element.length() < 2)
    {
        if (element > "9")
            return false;
        else
            return true;
    }
    else
        return true;
}

void displayElementsOfBoard(string element, int& column, int digits)
{
    int lengthOfElement = element.length();

    if (isDigit(element) == false)
    {
        for (int actualColumn = lengthOfElement; actualColumn < digits; ++actualColumn)
        {
            cout << " ";
        }

        cout << element;

        column += digits - 1;
    }
    else
    {
        if (lengthOfElement < digits)
        {
            for (int actualColumn = lengthOfElement; actualColumn < digits; ++actualColumn)
            {
                cout << " ";
            }
        }

        cout << element;

        column += digits - 1;
    }
}

void displayBoard(vector<vector<string>>board)
{
    size_t boardSize = board.size();
    int digits = howManyDigits(boardSize*boardSize);
    int graphicSize = digits * boardSize
        + boardSize - 1;
    int whichElement = 0;
    for (int row = 0;row < 2 * boardSize-1;++row)
    {
        if (isOx(row+1, 2*boardSize))
        {
            for (int column = 0;column < graphicSize;++column)
            {
                cout << "-";
            }
            cout << "\n";
            continue;
        }
        for (int column = 0;column < graphicSize;++column)
        {

            if (isOy(column+1,digits,graphicSize))
            {
                cout << "|";
            }
            else
            {
                string element = board[whichElement/boardSize][whichElement%boardSize];
                displayElementsOfBoard(element,column,digits);
                ++whichElement;
            }
        }
        cout << "\n";
    }
    
}

bool isValidCharacter(string character)
{
    if (character !="O" && character!="X")
        return false;
    else
        return true;
}

bool isValidElement(int boardSize, int element)
{
    if (boardSize * boardSize < element)
        return false;
    return true;
}

bool doesElementExist(vector<vector<string>>board, int whichElement)
{
    int boardSize = board.size();
    if (!isDigit(board[(whichElement - 1) / boardSize][(whichElement - 1) % boardSize]))
        return true;
    else
        return false;
}

int previousNumberOfElement;
string previousCharacter;

bool isGoodType(string element)
{
    int elementLength = element.length();
    for (int position = 0;position < elementLength;++position)
    {
        if (element[position] > '9')
        {
            return false;
        }
    }
    return true;
}

void boardChange(vector<vector<string>>&board)
{
    int boardSize = board.size();
    string specialCharacter;
    string whichElement1;

 
    cout << "Gdzie chcesz umiescic O lub X? (Podaj wd wzoru O 11)"<<"\n";
    cin >> specialCharacter>>whichElement1;
    if (!isGoodType(whichElement1))
    {
        cout << "Zle wpisales liczbe"<<"\n";
        boardChange(board);
    }
    else
    {
        int whichElement = stoi(whichElement1);
        if (!isValidCharacter(specialCharacter) || !isValidElement(boardSize, whichElement) || (specialCharacter.size() > 1) || doesElementExist(board, whichElement))
        {
            cout << "Pomyliles sie" << "\n";
            boardChange(board);
        }
        else
        {
            board[(whichElement - 1) / boardSize][(whichElement - 1) % boardSize] = specialCharacter;
            previousNumberOfElement = whichElement;
            previousCharacter = specialCharacter;
        }
    }

}

bool isWinOx(vector<vector<string>>board)
{
    int boardSize = board.size();
    int row = (previousNumberOfElement-1) / boardSize;
    for (int column = 0;column < boardSize;++column)
    {
        if (isDigit(board[row][column])||previousCharacter!= board[row][column])
            return false;
    }
    return true;
}

bool isWinOy(vector<vector<string>>board)
{
    int boardSize = board.size();
    int column = (previousNumberOfElement - 1) % boardSize;
    for (int row = 0;row < boardSize;++row)
    {
        if (isDigit(board[row][column])||previousCharacter != board[row][column])
            return false;
    }
    return true;
}
//przekatna o rozmiarze BoardSize jest tylko 1(najwieksza przekatna)
bool isWinDiagonalLR(vector<vector<string>>board)
{
    int boardSize = board.size();
    for (int row = 0;row < boardSize;++row)
    {
        if (isDigit(board[row][row]) || previousCharacter != board[row][row])
            return false;
    }
    return true;
}

bool isWinDiagonalRL(vector<vector<string>>board)
{
    int boardSize = board.size();
    for (int row = boardSize-1;row>=0;--row)
    {
        if (isDigit(board[boardSize - 1-row][row]) || previousCharacter != board[boardSize - 1 - row][row])
            return false;
    }
    return true;
}

bool isWin(vector<vector<string>>board)
{
    if (isWinOx(board)||isWinOy(board)||isWinDiagonalLR(board)||isWinDiagonalRL(board))
        return true;
    else
        return false;
}

void ticTacToe(vector<vector<string>>board)
{
    int boardSize = board.size();
    displayBoard(board);
    for (int ruch = 1;ruch> 0;++ruch)
    {
        boardChange(board);
        displayBoard(board);

        if (isWin(board))
        {
            cout << "Wygrywa gracz " << previousCharacter << "\n";
            break;
        }
            
        if (ruch == boardSize * boardSize)
        {
            cout<< "Remis"<< "\n";
            break;
        }
            
    }
    
}

int main()
{
    int boardSize = gameBeggining();
    vector<vector<string>>vectorBoard = board(boardSize);
    ticTacToe(vectorBoard);
}