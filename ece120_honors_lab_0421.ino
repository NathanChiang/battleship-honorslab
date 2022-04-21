#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GrayOLED.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>


int d_ship_remain[] = {42,43,44,45};// LED lights, in setup
int player_turn = 31;
int warning = 46;
int player_win = 47;
int computer_win = 48;
int d_pin[] = {32,33,34,35,36,37,38,39};  // An array of pins that are connected to the button input
int primitive[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The initial array that will be swapped randomly later by the generate function
int computer_board[16]; // Declare the computer board
/* Initialize the computer board
 *  0 means empty space with no ship
 *  1 means ship placed but not sunken
 *  2 means ship sunken
 *  3 means empty space chosen
*/

int player_board[16]; // Same case for the player board
/* A function that swap the primitive array
 * The modified array will then be the move of the computer
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
// This function translate the input from the button into the index for either the player board or the computer board
int translation(int arr[]) {
  int num = 0;
  for (int i = 0; i < 8; i++) {
    if (digitalRead(arr[i]) == LOW) {
      if (i < 4) {
        num += (i+1);
      } else {
        num += (i-4)*4;
      }
    }
  }
  return num;
}
void setup() {
  for (int i = 0; i < 16; i++) {
    computer_board[i] = 0;
  }
  for (int i = 0; i < 16; i++) {
    player_board[i] = 0;
  }
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
    int select = translation(d_pin);
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
boolean player_active = true; //player always starts first
int count = 16; // make sure the sequence of the computer attacks
void loop() {
  // put your main code here, to run repeatedly:
  if (player_active) {
    int attack = 0;
    digitalWrite(player_turn, HIGH);
    while (true) {
      if (translation(d_pin) > 0) {
        attack = translation(d_pin) - 1;
        break;
      }
    }
    if (computer_board[attack] == 1) {
      computer_board[attack] == 2;
      int two = 0;
      for (int i = 0; i < 16; i++) {
        if (computer_board[i] == 2) {
          two++;
        }
        if (two == 4) {
          digitalWrite(player_win, HIGH);
          delay(2000);
          return;
        }
      }
    } else if (computer_board[attack] == 0) {
      computer_board[attack] == 3;
      player_active == false;
    } else {
      digitalWrite(warning, HIGH);
      delay(1000);
    }
  } else {
    int option = primitive[count] - 1;
    count++;
    int two = 0;
    if (player_board[option] == 1) {
      player_board[option] == 2;
      for (int i = 0; i < 16; i++) {
        if (player_board[i] == 2) {
          two++;
        }
        if (two == 4) {
          digitalWrite(computer_win, HIGH);
          delay(2000);
          return;
        }
      }
    } else if (computer_board[option] == 0) {
      player_board[option] == 3;
      player_active == true;
    } else {
      digitalWrite(warning, HIGH);
      delay(1000);
    }
  }
}
