/*
#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>x
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>
*/

int d_ship_remain[] = {42,43,44,45};// LED lights, in setup
int warning = 46;
int player_win = 47;
int computer_win = 48;
int d_pin[] = {32,33,34,35,36,37,38,39};  // An array of pins that are connected to the button input
int primitive[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The initial array that will be swapped randomly later by the generate function
/* Initialize the computer board
 *  0 means empty space with no ship
 *  1 means ship placed but not sunken
 *  2 means ship sunken
 *  3 means empty space but chosen
*/
int computer_board[16];
int player_board[16];

/* 
int generate(array)
A function that swaps the entries of the primitive array.
The modified array will then be the predetermined moves of the computer
 */
void generate(int arr[]) {
    int tmp;
    int arbit_1;
    int arbit_2;
    for (int i = 0; i < 32; i++) {
        arbit_1 = random(16);
        arbit_2 = random(16);
        tmp = arr[arbit_1];
        arr[arbit_1] = arr[arbit_2];
        arr[arbit_2] = tmp;
    }
}

/*
int readInput(pinNumber)
Translates the input on an digital pin. If the pin is LOW, that means an
input was detected, and that would be equal to 1. Otherwise, the function
will return 0
*/
int readInput(int pin) {
    if (digitalRead(pin) == LOW) {
        return 1;
    } else {
        return 0;
    }
}

/*
int translate(array)
This function takes the input array from the button matrix and translates
the input to determine which spot on the board was picked. It is used in conjuction
with the readInput function to translate the input from the button matrix
*/
int translateInput(int arr[]) {
    int position = 0;
    int counter = 0;
    for (int i = 0; i < 8; i++) {
        if (arr[i] == 1) {
            if (i < 4) {
                position += i;
                counter++;
            } else {
                position += (i-4)*4;
                counter++;
            }
        }
    }
    // For added redundancy; should only return a position if two 1's are detected
    if (counter == 2) {
        return position;
    } else {
        return 17;
    }
}

void reset(int arr[]) {
    for (int i = 0; i < len(arr); i++) {
        arr[i] = 0;
    }
}

/*
void setup()
Built-in Arduino function that is used to initialize any data and arrays needed
for game play. See block diagram and flow chart for more information on how this program
runs
*/
void setup() {
    //Resets all board positions to 0
    reset(computer_board);
    reset(player_board);
    // put your setup code here, to run once:
    pinMode(player_turn, OUTPUT);
    digitalWrite(player_turn, LOW);
    for (int i = 0; i < 8; i++) {
        pinMode(d_pin[i], INPUT_PULLUP);
    }
    for (int i = 0; i < 8; i++) {
        pinMode(d_ship_remain[i], OUTPUT);
        digitalWrite(d_ship_remain[i], LOW);
    }
    generate(primitive);
    for (int i = 0; i < 4; i++) {
        int buff = random(1,5);
        computer_board[primitive[i*4 + buff]] = 1;
    }
    digitalWrite(player_turn, HIGH);
    for (int i = 0; i < 8; i++) {
        digitalWrite(d_ship_remain[i], HIGH);
    }
    int count = 0;
    int light_off = 41;
    while (count < 4) {
        /*read the button by bit
        *throw it into the translation array and get the index
        *you will derive a number used for an index
        *check the index in the player_board
        *if the value of the index is 0, modify the index to 1, and count++
        *if not, continue
        *In the end of this while loop, the player_board will have four index that are 1
        */
        int ship = 0;
        int select = translateInput(d_pin);
        player_board[select - 1] = 1;
        for (int i = 0; i < 16; i++) {
            ship += player_board[i];
        }
        if (ship > count) {
            count++;
            digitalWrite(d_ship_remain[light_off + count], LOW);
        }
    }
}

/*
void loop()
Main loop in which the arduino runs through the simplified battleship game.
See block diagram and flow chart for more information about the folow of the
program. This loop should only loop through for a maximum of 16 proper turns.
*/
//Keeps track of turn (player=true)/(computer=false)
boolean player_active = true; //player always starts first
//Computer variables
int compIndex = 0; // keeps track of computer attacks
int compHit = 0;
boolean computerWin = false;
//Player variables
int[] button = int[8];
int playerHit = 0;
boolean playerWin = false;
int position = 17; //initialize at an out of board position
void loop() {
    while (computerWin || playerWin) {
        if (computerWin) {
            digitalWrite(computer_win, HIGH);
            Serial.print("You lose! Computer is the winner!!");
            return;
        } else if (playerWin) {
            digitalWrite(player_win, HIGH);
            Serial.print("Player has won!!");
            return;
        }
    }

    while (digitalRead(30) == LOW && player_active) {
        button[0] = readInput(31);
        button[1] = readInput(32);
        button[2] = readInput(33);
        button[3] = readInput(34);
        button[4] = readInput(35);
        button[5] = readInput(36);
        button[6] = readInput(37);
        button[7] = readInput(38);
        Serial.print("Here\n");
    } else {
        if (!player_active) { //ignoring this while loop if it is still computer turn
            break;
        }
        delay(500);
    }
    position = translateInput(button); //returns an index in the board array

    //The following code is for the player turn

    /* 
        0 means empty space with no ship
        1 means ship placed but not sunken
        2 means ship sunken
        3 means empty space but chosen
    */
    if(player_active) {
        if (position <=  15 && position >= 0) { //checks if there was a valid turn
            if (computer_board[position] ==  0 || computer_board[position] ==  1) {
                if (computer_board[position] ==  1) { //player has hit a ship
                    computer_board[position] ==  2;
                    player_active = true; //player gets another turn
                    playerHit++;
                    Serial.print("Battleship! Go again!\n");
                    if (playerHit == 4) { //win conditional
                        playerWin = true;
                    }
                } else if (computer_board[position] ==  0) { //player has missed ship
                    computer_board[position] ==  3;
                    player_active = false; //change turns
                    Serial.print("You missed! :( \n");
                } else {
                    player_active = true;
                    Serial.print("There was an error!\n");
                }
            } else {
                digitalWrite(warning, HIGH);
                Serial.print("Spot had already been chosen. Try Again!\n");
                player_active = true;
            }
        } else {
            Serial.print("Turn failed. Try again!\n");
            player_active = true;
        }
    }

    //The following code is for the player turn

    if (!player_active) { //computer only goes if player does not get another turn/try again
        int attack = computer_board[compIndex];
        compIndex++;

        if (player_board[position] ==  1) { //computer has hit a ship
            player_board[position] ==  2;
            player_active = false; //computer gets another turn
            compHit++;
            if (compHit == 4) { //win conditional
                computerWin = true;
            }
        } else if (player_board[position] ==  0) { //computer has missed ship
            player_board[position] ==  3;
            player_active = true; //change turns
        } else {
            player_board = true;
            Serial.print("There was an error!\n");
        } //Note: don't need the additional layers of checks like with player turn
    }

    //Adding redundacies so that the loop does not unintentionally play a turn
    position = 17;
    reset(button);
}
