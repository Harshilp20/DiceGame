//  Name: Harshil Patel
//  Course: Data Structures CS2028C
//  Assignment: Lab 1

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <fstream>
#include <stdlib.h>

#define MAX_PLAYERS 10

using namespace std;


//Class representing an N-sided die
class Dice{
private:
    int numSides;
public:
    // Perform a single roll of the die
    Dice(int sides){
        srand((unsigned)time(0));
        numSides = sides;
    }
    // Roll the dice from 1 to numSides
    int roll() {
        return (rand() % numSides + 1 );
    }
    int GetDiceType(){ return numSides; }
};

//Class representing a Game Player

class Player{
private:
    string name;
    int score;
    bool eliminated;
public:
    //Player constructor
    Player(string n = NULL, int s = 0){
        name = n;
        score = s;
    }
    // Getter : Player Name
    string getName() {
        return name;
    }
    // Getter : Score
    int getScore() {
        return score;
    }
    // Increases the player's score by a specified value
    void addToScore(int val){
        score += val;
    }
    
    //Set eliminated variable to true
    void Eliminate(){ eliminated = true; }
    
    bool isEliminated(){ return eliminated; }
};


//Abstract class DiceGame

class DiceGame{
protected:
    Player *players[MAX_PLAYERS];
    Dice *dice;
    int numPlayers;
    int numDice;
public:
    //    Pure virtual function
    virtual void play() = 0;
    
    //Initialize all players by entering name and score
    void initPlayers(){
        string name;
        int score;
        Player *objPointer = nullptr;
        for (int i = 0; i < numPlayers; i++){
            cout<<"Enter Name of Player "<<i + 1<<": ";
            cin>>name;
            cout<<"\nEnter Score of Player "<<i + 1<<": ";
            cin>>score;
            cout<<"\n\n";
            objPointer = new Player(name, score);
            players[i] = objPointer;
        }
    }
    
    //Display scores in console
    void displayScores(){
        cout<<"=====Scores=====\n";
        for (auto player : players){
            if (!(player == NULL)){
                cout<<player->getName()<<" : "<<player->getScore()<<endl;
            }
        }
    }
    
    //create/update ScoreCard.txt file
    void writeScoresToFile(){
        ofstream myfile("ScoreCard.txt");
        
        //Add the names and scores of each player in file
        if(myfile.is_open()){
            for (auto player : players){
                if(!(player == NULL)){
                    myfile<<player->getName()<<" : "<<player->getScore()<<endl;
                }
            }
            myfile.close();
        }
        else{
            cout<<"Unable to open file.\n";
        }
    }
    
    void GetHighestScore(){
        string line;
        int scoreValue = 0;
        int highestScore = 0;
        //Open ScoreCard.txt file
        ifstream myfile("ScoreCard.txt");
        
        //Logic to read lines of the file and find highest score
        if (myfile.is_open()){
            while (getline(myfile, line)){
                //Parse only the score from each line in the file
                scoreValue = atoi((line.substr(line.find(": ") + 1)).c_str());
                if (scoreValue > highestScore){
                    highestScore = scoreValue;
                }
            }
            //Print the highest score in the console
            cout<<"Highest Score: "<<highestScore<<endl<<endl;
        }
        else{
            cout<<"Unable to open file.\n";
        }
    }
};

//Knock Out Game (class)
class KnockOut : public DiceGame{
private:
    int knockOutScore = 0;
    
    //Set the knock out score
    void SetKOScore(){
        for (int i = 0; i < numDice; i++){
            knockOutScore += dice->roll();
        }
    }
public:
    //Constructor
    KnockOut(int numPlayers, int numDice, int numSides){
        this->numPlayers = numPlayers;
        this->numDice = numDice;
        dice = new Dice(numSides);
    }
    
    //Get knock out score
    void GetKOScore(){ cout<<"KnockOutScore: "<< knockOutScore<<endl<<endl; }
    
    //roll add dice and return the total
    int RollDice(){
        int total = 0;
        for (int i = 0; i < numDice; i++){
            total += dice->roll();
        }
        return total;
    }
    
    //play method
    void play(){
        int rollSum;
        
        //Set the Knock out score and print it to the console
        SetKOScore();
        GetKOScore();
        
        //conditional variable
        int sizeOfArray = numPlayers;
        
        //Game logic
        while(sizeOfArray > 1){
            for (int i = 0; i < numPlayers; i++){
                //if player is elimated, skip that iteration
                if (players[i]->isEliminated()){
                    continue;
                }
                else{
                    rollSum = RollDice();
                    cout<<players[i]->getName()<<" rolls a total of: "<<rollSum<<endl;
                    
                    if (rollSum == knockOutScore){
                        //eliminate player if sum equals knockout score
                        cout<<players[i]->getName()<<" is KNOCKED OUT!!!\n\n";
                        players[i]->Eliminate();
                        sizeOfArray -= 1;
                        //Stops going through the for loop if we have reached 1 player
                        if (sizeOfArray == 1){
                            break;
                        }
                    }
                }
            }
        }
        
        //Display Winner by checking which player is not elimated
        for (int i = 0; i < numPlayers; i++){
            if (players[i]->isEliminated()){
                continue;
            }
            players[i]->addToScore(10);
            cout<<players[i]->getName()<<" is the winner!!!"<<endl;
        }
        cout<<"Knock Out Game Completed\n\n";
    }
};


class BostonDiceGame : public DiceGame{
private:
    //roll all remaining dice and get the highest value
    int getHighestRoll(int num){
        int highestRoll = 0;
        int currentRoll = 0;
        int previousRoll = 0;
        for(int i = 0; i < num; i++){
            currentRoll = dice->roll();
            if(currentRoll > highestRoll){
                highestRoll = currentRoll;
            }
            previousRoll = currentRoll;
        }
        return highestRoll;
    }
    
public:
    //Constructor
    BostonDiceGame(int numPlayers, int numDice, int numSides){
        this->numPlayers = numPlayers;
        this->numDice = numDice;
        dice = new Dice(numSides);
    }
    
    //play method
    void play(){
        int diceNumber = numDice;
        int roll;
        
        //Game logic to find the sum of all highest dice rolls as we decrement the dice.
        for (int i = 0; i < numPlayers; i++){
            while(diceNumber > 0){
                roll = getHighestRoll(diceNumber);
                players[i]->addToScore(roll);
                cout<<players[i]->getName()<<" rolls: "<<roll<<endl;
                diceNumber -= 1;
            }
            cout<<players[i]->getName()<<" rolls a total of: "<<players[i]->getScore()<<" points\n\n";
            diceNumber = numDice;
        }
        
        //find the highest score from all the players
        int highestTotal = players[0]->getScore();
        int player = 0;
        for (int i = 1; i < numPlayers; i++){
            if ((players[i]->getScore()) > highestTotal){
                highestTotal = players[i]->getScore();
                player = i;
            }
        }
        
        //display winner
        cout<<endl<<players[player]->getName()<<" Wins!!!\n";
        cout<<"Boston Dice Roll Game Completed\n\n";
    }
};


int main() {
    // Base class pointer
    DiceGame *game;
    
    //Ask for game type
    int gameType = 0;
    cout<<"Hello, which dice game would you like to play!\nPress 1 for Knock Out or 2 for Boston Dice Game: ";
    cin>>gameType;
    while((gameType != 1)&&(gameType != 2)){
        cout<<"Invalid Entry, Please Try Again: ";
        cin>>gameType;
    }
    
    //Ask for number of players
    int numPlayers;
    cout<<"\nEnter the number of Players: ";
    cin>>numPlayers;
    
    //Ask for number of dice
    int numDice;
    cout<<"\nEnter the number of Dice: ";
    cin>>numDice;
    
    //Ask for number of sides (type) on dice
    int numSides;
    cout<<"\nEnter the number of sides on the dice: ";
    cin>>numSides;
    cout<<"\n\n";
    
    //initialize game pointer based on input
    if(gameType == 1){
        game = new KnockOut(numPlayers, numDice, numSides);
    }
    else{
        game = new BostonDiceGame(numPlayers, numDice, numSides);
    }
    
    //initialize players and play the game
    game->initPlayers();
    game->play();
    
    //create a file to write the score to
    game->writeScoresToFile();
    
    //read the file, find the highest score, and display it in the console
    game->GetHighestScore();
}
