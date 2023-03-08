#include <iostream>
#include <fstream>

using namespace std;

class Othello
{
    char board[8][8], undoboard[8][8];
    int player;
    int winner;
    int remainingCoinsBlack, undoRemainingCoinsBlack;
    int remainingCoinsWhite, undoRemainingCoinsWhite;
    int coinsCountOnBoardBlack,undoCoinsOnBoardBlack;
    int coinsCountOnBoardWhite, undoCoinsOnBoardWhite;

public:
    void newgame()
    {
        player = 1;
        winner = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                board[i][j]='-';
                undoboard[i][j]='-';
            }
        }
        board[3][3] = 'B';
        board[3][4] = 'W';
        board[4][3] = 'W';
        board[4][4] = 'B';
        remainingCoinsBlack=30;
        remainingCoinsWhite=30;
        coinsCountOnBoardBlack=2;
        coinsCountOnBoardWhite=2;
    }
    bool continueGame()
    {
        ifstream inputFile("game.txt", ios :: in);
        if(!inputFile)
        {
            cout<<"no game has been saved.\n";
            return false;
        }
        inputFile>>player;
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                inputFile>>board[i][j];
        inputFile>>remainingCoinsBlack>>remainingCoinsWhite>>coinsCountOnBoardBlack>>coinsCountOnBoardWhite;
        winner = 0 ;
        inputFile.close();
        return true;
    }
    bool restart()
    {
        this->newgame();
        return true;
    }
    void saveFile()
    {

        ofstream outputFile("game.txt", ios::out);
        outputFile<<player<<endl;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                outputFile<<board[i][j];
                if(j!=7)
                    outputFile<<" ";
            }
            outputFile<<endl;
        }
        outputFile<<remainingCoinsBlack<<endl<<remainingCoinsWhite<<endl
                 <<coinsCountOnBoardBlack<<endl<<coinsCountOnBoardWhite;
        outputFile.close();
    }
    void savewinnersFile()
    {
        ifstream inputFile("gameslist.txt", ios::in);
        char **games;
        int c;
        if(inputFile)
        {
            inputFile>>c;
            games = new char* [c];
            for(int i=0; i<c; i++)
                games[i] = new char[5];
            for(int i=0; i<c; i++)
                inputFile>>games[i];
        }
        else
        {
            c=0;
        }
        inputFile.close();
        if (getWinner() != 0)
        {
            if(getWinner()==1)
            {
                cout << "The winner is Player- Black" << ".\n";

                ofstream outputFile("gameslist.txt", ios::out);
                if (c==10)
                {
                    outputFile<<c<<endl;

                    for(int i=1; i<c; i++)
                        outputFile<<games[i]<<endl;
                    outputFile<<"Black";
                    outputFile.close();
                }
                else
                {
                    outputFile<<c+1<<endl;

                    for(int i=0; i<c; i++)
                        outputFile<<games[i]<<endl;
                    outputFile<<"Black";
                    outputFile.close();
                }
            }
            else
            {
                cout << "The winner is Player- White" << ".\n";

                ofstream outputFile("gameslist.txt", ios::out);
                if (c==10)
                {
                    outputFile<<c<<endl;

                    for(int i=1; i<c; i++)
                        outputFile<<games[i]<<endl;
                    outputFile<<"White";
                    outputFile.close();
                }
                else
                {
                    outputFile<<c+1<<endl;

                    for(int i=0; i<c; i++)
                        outputFile<<games[i]<<endl;
                    outputFile<<"White";
                    outputFile.close();
                }
            }
        }
        else
        {
            cout << "The match ended in a draw.\n";

            ofstream outputFile("gameslist.txt", ios::out);
            if (c==10)
            {
                outputFile<<c<<endl;

                for(int i=1; i<c; i++)
                    outputFile<<games[i]<<endl;
                outputFile<<"isTie";
                outputFile.close();
            }
            else
            {
                outputFile<<c+1<<endl;

                for(int i=0; i<c; i++)
                    outputFile<<games[i]<<endl;
                outputFile<<"isTie";
                outputFile.close();
            }
        }
    }
private:
    bool checkFlip(char colour, int row, int col, int deltaRow, int deltaCol)
    {
        char oppColour = 'W';
        if (colour == 'W')
        {
            oppColour = 'B';
        }

        if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8))
        {
            return false;
        }

        if (board[row][col] == oppColour)
        {
            while ((row >= 0) && (row < 8) && (col >= 0) && (col < 8))
            {
                row += deltaRow;
                col += deltaCol;
                if (board[row][col] == '-')
                {
                    return false;
                }
                if (board[row][col] == colour)
                {
                    return true;
                }
            }
        }
        return false;
    }
    void changeCoins(char colour, int row, int col, int deltaRow, int deltaCol)
    {
        char oppColour = 'W';
        if (colour == 'W')
        {
            oppColour = 'B';
        }

        while (board[row][col] == oppColour)
        {
            board[row][col] = colour;
            if(colour=='W')
            {
                coinsCountOnBoardBlack--;
                coinsCountOnBoardWhite++;
            }
            else
            {
                coinsCountOnBoardBlack++;
                coinsCountOnBoardWhite--;
            }
            row += deltaRow;
            col += deltaCol;
        }
        return;
    }

public:
    bool isValidMove(int row, int col)
    {
        if ((row < 0) || (row >= 8) || (col < 0) || (col >= 8))
        {
            return false;
        }

        if (board[row][col] != '-')
        {
            return false;
        }

        char colour = 'W';
        if (getPlayer() == 1)
        {
            colour = 'B';
        }

        // Check Up
        if (checkFlip(colour, row - 1, col, -1, 0))
        {
            return true;
        }
        // Check Down
        if (checkFlip(colour, row + 1, col, 1, 0))
        {
            return true;
        }
        // Check Left
        if (checkFlip(colour, row, col - 1, 0, -1))
        {
            return true;
        }
        // Check Right
        if (checkFlip(colour, row, col + 1, 0, 1))
        {
            return true;
        }
        // Check Down-Right
        if (checkFlip(colour, row + 1, col + 1, 1, 1))
        {
            return true;
        }
        // Check Down-Left
        if (checkFlip(colour, row + 1, col - 1, 1, -1))
        {
            return true;
        }
        // Check Top-Right
        if (checkFlip(colour, row - 1, col + 1, -1, 1))
        {
            return true;
        }
        // Check Top-Left
        if (checkFlip(colour, row - 1, col - 1, -1, -1))
        {
            return true;
        }

        return false;
    }
    void makeMove(int row, int col)
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                undoboard[i][j] = board[i][j];
        undoRemainingCoinsWhite = remainingCoinsWhite;
        undoRemainingCoinsBlack = remainingCoinsBlack;
        undoCoinsOnBoardBlack = coinsCountOnBoardBlack;
        undoCoinsOnBoardWhite = coinsCountOnBoardWhite;
        char colour = 'W';
        if (getPlayer() == 1)
        {
            colour = 'B';
        }
        board[row][col] = colour;
        if(colour=='W')
        {
            coinsCountOnBoardWhite++;
            remainingCoinsWhite--;
        }
        else
        {
            coinsCountOnBoardBlack++;
            remainingCoinsBlack--;
        }

        // Check Up
        if (checkFlip(colour, row - 1, col, -1, 0))
        {
            changeCoins(colour, row - 1, col, -1, 0);
        }
        // Check Down
        if (checkFlip(colour, row + 1, col, 1, 0))
        {
            changeCoins(colour, row + 1, col, 1, 0);
        }
        // Check Left
        if (checkFlip(colour, row, col - 1, 0, -1))
        {
            changeCoins(colour, row, col - 1, 0, -1);
        }
        // Check Right
        if (checkFlip(colour, row, col + 1, 0, 1))
        {
            changeCoins(colour, row, col + 1, 0, 1);
        }
        // Check Down-Right
        if (checkFlip(colour, row + 1, col + 1, 1, 1))
        {
            changeCoins(colour, row + 1, col + 1, 1, 1);
        }
        // Check Down-Left
        if (checkFlip(colour, row + 1, col - 1, 1, -1))
        {
            changeCoins(colour, row + 1, col - 1, 1, -1);
        }
        // Check Top-Right
        if (checkFlip(colour, row - 1, col + 1, -1, 1))
        {
            changeCoins(colour, row - 1, col + 1, -1, 1);
        }
        // Check Top-Left
        if (checkFlip(colour, row - 1, col - 1, -1, -1))
        {
            changeCoins(colour, row - 1, col - 1, -1, -1);
        }
        player = -1 * player;
        return;
    }
    void displayBoard()
    {
        int sum=0;
        for (int i = 0; i < 8; i++)
        {
            cout << i << "\t";
            for (int j = 0; j < 8; j++)
            {
                if(isValidMove(i,j))
                {
                    sum++;
                    cout<<'*'<<"\t";
                }

                else
                    cout << board[i][j] << "\t";
            }
            cout << "\n";
        }
        if(sum==0)
        {
            player=-1*player;
        }
        cout << "\t0\t1\t2\t3\t4\t5\t6\t7\n";
        cout << "Coins on board for Player Black : " << coinsCountOnBoardBlack << "\n";
        cout << "Coins on board for Player White : " << coinsCountOnBoardWhite << "\n";
        cout << "remaining Coins for Player Black : " << remainingCoinsBlack << "\n";
        cout << "remaining Coins for Player White : " << remainingCoinsWhite << "\n";
        cout << "harkat mojaz : "<<sum<<"\n";
        return;
    }
    bool isGameOver()
    {
        if (coinsCountOnBoardBlack == 0)
        {
            winner = -1;
            return true;
        }
        else if (coinsCountOnBoardWhite == 0)
        {
            winner = 1;
            return true;
        }
        else if (remainingCoinsBlack == 0 || remainingCoinsWhite == 0)
        {
            if (coinsCountOnBoardBlack > coinsCountOnBoardWhite)
            {
                winner = 1;
            }
            else if (coinsCountOnBoardBlack < coinsCountOnBoardWhite)
            {
                winner = -1;
            }
            else
            {
                winner = 0;
            }
            return true;
        }
        return false;
    }
    int getPlayer()
    {
        if (player == -1)
        {
            return 2;
        }
        return player;
    }
    int getOppPlayer()
    {
        if (player == -1)
        {
            return 1;
        }
        return 2;
    }
    int getWinner()
    {
        return winner;
    }
    void undo()
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                board[i][j] = undoboard[i][j];

        player = -1 * player;
        coinsCountOnBoardBlack = undoCoinsOnBoardBlack;
        coinsCountOnBoardWhite = undoCoinsOnBoardWhite;
        remainingCoinsBlack = undoRemainingCoinsBlack;
        remainingCoinsWhite = undoRemainingCoinsWhite;
    }
};

void resumExistingGame()
{
    Othello Game;
    if(Game.continueGame()){
        cout << "The Game has started.\n";
        Game.displayBoard();
        int row;
        int col;
        bool restart=false;
        bool admitorundo;
        while (!Game.isGameOver())
        {
            if(Game.getPlayer()==1)
                cout << "Player Black's turn.\n";
            else
                cout<<"Player White's turn\n";
            do
            {
                cout<<"enter 10 for restart\nenter 11 to exit\nenter the row number to continue:\n";
                cin >> row;
                if(row==10)
                {
                    Game.restart();
                    restart=true;
                    break;
                }
                if(row==11)
                {
                    cout<<"\nsave the game?1->yes 2->no";
                    int op;
                    cin>>op;
                    if(op==1)
                    {
                        Game.saveFile();
                        Game.savewinnersFile();
                    }
                    exit(1);
                }
                cout << "Enter the Column Number: ";
                cin >> col;
            } while(row>7 || row<0 || col>7 || col<0 );
            if(restart)
            {
                restart=false;
                system("cls");
                cout<<"restarted!\n";
                Game.displayBoard();
                continue;
            }
            if (Game.isValidMove(row, col))
            {
                Game.makeMove(row, col);
                system("cls");
                Game.displayBoard();
                cout<<"\n\tyour move is "<<row<<col<<" are you sure?\n0->no undo it\t1->yes, go to other player's turn :";
                cin>>admitorundo;
                if(admitorundo == 0)
                {
                    Game.undo();
                    system("cls");
                    cout<<"undo is done!\n";
                    Game.displayBoard();
                }
            }
            else
            {
                system("cls");
                cout << "Invalid Move. Try Again.\n";
                Game.displayBoard();
            }

        }

        cout << "Game Over.\n";
        Game.savewinnersFile();}
    else
        return;
}
void startNewGame()
{
    Othello Game;
    Game.newgame();
    cout << "The Game has started.\n";
    Game.displayBoard();
    int row;
    int col;
    bool restart=false;
    bool admitorundo;
    while (!Game.isGameOver())
    {
        if(Game.getPlayer()==1)
            cout << "Player Black's turn.\n";
        else
            cout<<"Player White's turn\n";
        do
        {
            cout << "Enter 10 to restart\nenter 11 to exit\nenter the row number to continue\n";
            cin >> row;
            if(row==10)
            {
                Game.restart();
                restart=true;
                break;
            }
            if(row==11)
            {
                cout<<"\nsave the game?1->yes 2->no";
                int op;
                cin>>op;
                if(op==1)
                {
                    Game.saveFile();
                    Game.savewinnersFile();
                }
                exit(1);
            }
            cout << "Enter the Column Number: ";
            cin >> col;
        } while(row>7 || row<0 || col>7 || col<0 );
        if(restart)
        {
            restart=false;
            system("cls");
            cout<<"restarted!\n";
            Game.displayBoard();
            continue;
        }
        if (Game.isValidMove(row, col))
        {
            Game.makeMove(row, col);
            system("cls");
            Game.displayBoard();
            cout<<"\n\tyour move is "<<row<<col<<" are you sure?\n0->no undo it\t1->yes, go to other player's turn :";
            cin>>admitorundo;
            if(admitorundo == 0)
            {
                Game.undo();
                system("cls");
                cout<<"undo is done!\n";
                Game.displayBoard();
            }
        }
        else
        {
            system("cls");
            cout << "Invalid Move. Try Again.\n";
            Game.displayBoard();
        }

    }

    cout << "Game Over.\n";
    Game.savewinnersFile();
}

void thelast10games()
{

    ifstream inputFile("gameslist.txt", ios::in);
    if(!inputFile)
    {
        cout<<"no record has been recorded!\n";
        return;
    }
    int c;
    inputFile>>c;
    cout<<"the last 10 games:\n";
    for(int i=0; i<c; i++)
    {
        char x[5];
        inputFile>>x;
        cout<<i+1<<"th game winner: "<<x;
        cout<<endl;
    }
    inputFile.close();
    int choice;
    cout<<"what do you want to do?1->resume the last game\n2->start a new game\n3->end program";
    cin>>choice;
    if(choice == 1)
    {
        resumExistingGame();
    }
    else if(choice == 2)
    {
        startNewGame();
    }
    else
        return ;
}
int main()
{
    int choice;
    cout<<"what do you want to do?\n1->see the last 10 games\n2->resume the last game\n3->start a new game\n4->end program";
    cin>>choice;
    if(choice == 1)
        thelast10games();
    else if(choice == 2)
    {
        resumExistingGame();
    }
    else if(choice == 3)
    {
        startNewGame();
    }
    else
        return 0;
}
