#include <iostream>
#include <string>
using namespace std;

#define RESET    "\033[0m"
#define BOLD     "\033[1m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"
#define CYAN     "\033[36m"
#define MAGENTA  "\033[35m"
#define WHITE    "\033[97m"
#define BG_BLUE  "\033[44m"
#define BG_GREEN "\033[42m"
#define BG_RED   "\033[41m"

class Player        //  Abstract Base Class
{
private:
    string name;
    int score;     

public:

    Player(string playerName)
    {
        name = playerName;
        score = 0;          
    }

    string getName()  const
    {
        return name;
    }
    int    getScore() const 
    {
        return score;
    }   

    void incrementScore()
    {
        score++;            
    }

    void resetScore()
    {
        score = 0;        
    }

    virtual string makeChoice() = 0;

    virtual ~Player() {}
};


class HumanPlayer : public Player
{
public:
    HumanPlayer(string playerName) : Player(playerName) {}

    string makeChoice()
    {
        char input;

        while (true)
        {
            cout << "  Your move (R = Rock, P = Paper, S = Scissors): ";
            cin >> input;

            if (input == 'R' || input == 'r') 
                return "Rock";
            else if (input == 'P' || input == 'p') 
                return "Paper";
            else if (input == 'S' || input == 's') 
                return "Scissors";
            else
                cout << RED << "  Invalid input! Please re-enter :( \n" << RESET;
        }
    }
};


class ComputerPlayer : public Player
{
public:
    ComputerPlayer(string playerName) : Player(playerName) {}

    virtual string makeChoice() = 0;

    virtual void setHumanMove(string move) {}

    virtual ~ComputerPlayer() {}
};


//  Level 1: EasyComputer (always picks Rock)
class EasyComputer : public ComputerPlayer
{
public:
    EasyComputer() : ComputerPlayer("Easy Level") {}

    string makeChoice() 
    {
        return "Rock";
    }
};


//  Level 2: MediumComputer (random)
class MediumComputer : public ComputerPlayer
{
public:
    MediumComputer() : ComputerPlayer("Medium Level") {}

    string makeChoice()
    {
        int r = rand() % 3;
        if (r == 0)
            return "Rock";
        if (r == 1) 
            return "Paper";
        return "Scissors";
    }
};


//  Level 3: HardComputer (counters human's last move)
class HardComputer : public ComputerPlayer
{
private:
    string HumanMove;

public:
    HardComputer() : ComputerPlayer("Hard Level") { HumanMove = ""; }

    void setHumanMove(string move) { HumanMove = move; }

    string makeChoice()
    {
        if (HumanMove == "Rock")     
            return "Paper";
        if (HumanMove == "Paper")   
            return "Scissors";
        if (HumanMove == "Scissors")
            return "Rock";

        int r = rand() % 3;
        if (r == 0) 
            return "Rock";
        if (r == 1) 
            return "Paper";
        return 
            "Scissors";
    }
};


class Game
{
private:
    HumanPlayer* human;
    ComputerPlayer* computer;
    int  totalRounds;
    int  powerUps = 1; 
    bool powerupactive = false;

    string determineWinner(string h, string c)
    {
        if (h == c) 
            return "Tie";
        if ((h == "Rock" && c == "Scissors") ||(h == "Paper" && c == "Rock") ||(h == "Scissors" && c == "Paper"))
            return "Human";
        return "Computer";
    }

public:
    Game() {}

    Game(HumanPlayer* h, ComputerPlayer* c, bool powerUp)
    {
        human = h;
        computer = c;
        totalRounds = 3;
        powerupactive = powerUp; 
    }

    bool getPowerUpActive() const
    {
        return powerupactive;
    }

    void askforPowerUp()
    {
        char c;
        cout << "Enter Y to activate your power-up, or N to skip: ";
        cin >> c;

        if (c == 'Y' || c == 'y')
        {
            powerupactive = true;
            cout << GREEN << "POWER UP ACTIVATED \n" << RESET;
        }
        else if (c == 'N' || c == 'n')
        {
            powerupactive = false;
        }
        else
        {
            cout << RED << "INVALID INPUT power up not activated.\n" << RESET;
        }
    }

    void playMatch()
    {
        int round = 0;

        while (human->getScore() <= 2 && computer->getScore() <= 2 && round < totalRounds)
        {
            round++;

            cout << "\n" << YELLOW << BOLD
                << "  ========================================\n"
                << "  Round " << round << " - Make your move!\n"
                << "  ========================================\n" << RESET;

            string hMove = human->makeChoice();
            computer->setHumanMove(hMove);
            string cMove = computer->makeChoice();
            string result = determineWinner(hMove, cMove);

            if (result == "Human")
            {
                if (powerupactive && powerUps == 1)
                {
                    human->incrementScore();
                    human->incrementScore();
                    powerUps = 0;
                    cout << BG_GREEN << WHITE << " POWER UP BONUS  +2 points! " << RESET << "\n";
                }
                else
                {
                    human->incrementScore();
                    if (powerupactive && powerUps == 0)
                        cout << YELLOW << " Power up already used \n" << RESET;
                }
            }
            else if (result == "Computer")
            {
                if (powerupactive && powerUps == 1)
                {
                    powerUps = 0;
                    cout << RED << "POWER UP WASTED! You lost the round (no bonus)\n" << RESET;
                }
                computer->incrementScore();
            }

            if (powerupactive == false && powerUps == 1)
                cout << CYAN << "  ( you still have 1 power up available use it before the next match!)\n" << RESET;

            displayRoundResult(round, hMove, cMove, result);
        }

        cout << "\n" << BOLD;
        if (human->getScore() > computer->getScore())
        {
            cout << GREEN
                << "  ===================================\n"
                << "  |  *** YOU WIN THE MATCH "
                << human->getScore() << "-" << computer->getScore()
                << "! Congratulations! ***  |\n"
                << "  ======================================\n" << RESET;
        }
        else if (computer->getScore() > human->getScore())
        {
            cout << RED
                << "  ===========================================\n"
                << "  |  --- " << computer->getName() << " wins "
                << computer->getScore() << "-" << human->getScore()
                << "! Better luck next time ---   |\n"
                << "  ===========================================\n" << RESET;
        }
        else
        {
            cout << CYAN
                << "  =============================================\n"
                << "  |  === It's a DRAW! "
                << human->getScore() << "-" << computer->getScore()
                << " - Well matched! ===       |\n"
                << "  =============================================\n" << RESET;
        }
    }

    void displayRoundResult(int round, string hMove, string cMove, string result)
    {
        cout << endl << CYAN << "---------------------------------------------" << RESET << endl;
        cout << CYAN << "| " << BOLD << WHITE << "Round " << round << " of " << totalRounds << CYAN << " |\n" << RESET;
        cout << CYAN << "---------------------------------------------" << RESET << "\n";
        cout << CYAN << "| " << YELLOW << "You chose: " << BOLD << hMove << CYAN << " |\n" << RESET;
        cout << CYAN << "| " << MAGENTA << "Computer chose: " << BOLD << cMove << CYAN << " |\n" << RESET;
        cout << CYAN << "---------------------------------------------" << RESET << "\n";

        if (result == "Human")
            cout << CYAN << "| " << BG_GREEN << WHITE << " YOU WIN THIS ROUND! " << RESET << CYAN << " |\n" << RESET;
        else if (result == "Computer")
            cout << CYAN << "| " << BG_RED << WHITE << " Computer wins this round! " << RESET << CYAN << " |\n" << RESET;
        else
            cout << CYAN << "| " << BG_BLUE << WHITE << " It's a TIE! " << RESET << CYAN << " |\n" << RESET;

        cout << CYAN << "| " << WHITE << "Score: "
            << GREEN << human->getName() << " " << human->getScore() << " - "
            << RED << computer->getName() << " " << computer->getScore()
            << CYAN << " |\n" << RESET;
        cout << CYAN << "---------------------------------------------" << RESET << "\n";
    }

    int chooseDifficulty()
    {
        int choice = 0;
        cout << "\n" << YELLOW << BG_BLUE
            << "   ------------------------------------------------- \n"
            << "  |       Choose Your Difficulty Level:             |\n"
            << "   ------------------------------------------------- \n" << RESET
            << YELLOW << "| " << GREEN << "1.  Easy " << YELLOW << "|\n"
            << "| " << CYAN << "2.  Medium " << YELLOW << "|\n"
            << "| " << RED << "3.  Hard " << YELLOW << "|\n"
            << BOLD << "  -------------------------------------------------\n" << RESET;

        askforPowerUp();

        while (choice < 1 || choice > 3)
        {
            cout << GREEN << "\n  Enter level (1, 2, or 3): " << RESET;
            cin >> choice;
            if (choice < 1 || choice > 3)
                cout << RED << "  Invalid! Please enter 1, 2, or 3.\n" << RESET;
        }
        return choice;
    }
};


void displayLevelHeader(int level, string compName)
{
    cout << "\n" << MAGENTA << BOLD
        << "  -------------------------------------------------\n" << RESET;
    if (level == 1) cout << GREEN << BOLD << "        Level 1: " << compName << " (Easy)\n" << RESET;
    else if (level == 2) cout << CYAN << BOLD << "        Level 2: " << compName << " (Medium)\n" << RESET;
    else                 cout << RED << BOLD << "        Level 3: " << compName << " (Hard)\n" << RESET;
    cout << MAGENTA << BOLD << "  -------------------------------------------------\n" << RESET
        << WHITE << "  Best of 3 - First to win 2 rounds wins the match!\n" << RESET;
}


int main()
{

    cout << "\n" << MAGENTA << BOLD
        << "  =====================================================\n"
        << "       ***  ROCK  PAPER  SCISSORS  ***\n"
        << "           ---  3 LEVEL CHALLENGE  ---\n"
        << "  =====================================================\n" << RESET;

    string playerName;
    cout << BOLD << "\n  Enter your name: " << RESET;
    cin >> playerName;

    Game g;
    char again = 'Y';

    while (again == 'Y' || again == 'y')
    {
        int level = g.chooseDifficulty();

        HumanPlayer* human = new HumanPlayer(playerName);
        ComputerPlayer* computer = nullptr;

        if (level == 1) 
            computer = new EasyComputer();
        else if (level == 2) 
            computer = new MediumComputer();
        else                
            computer = new HardComputer();

        displayLevelHeader(level, computer->getName());

        Game game(human, computer, g.getPowerUpActive());
        game.playMatch();

        cout << "\n" << YELLOW << BOLD
            << "   ---------------------------------- \n"
            << "  |  Play again? (Y = Yes / N = No)  |\n"
            << "   ---------------------------------- \n" << RESET
            << BOLD << "  Your choice: " << RESET;
        cin >> again;

        delete human;
        delete computer;
    }

    cout << "\n" << CYAN << BOLD
        << " ===============================================\n"
        << " Thanks for playing, " << playerName << "!\n"
        << " ===============================================\n" << RESET;

    return 0;
}