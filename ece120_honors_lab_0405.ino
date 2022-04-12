
int d_ship_remain[] = {12,13,14,15};
int player_turn = 16;
int d_pin[] = {2,3,4,5,6,7,8,9};  // An array of pins that are connected to the button input
int primitive[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}; // The initial array that will be swapped randomly later by the generate function
int computer_board[16]; // Declare the computer board
/* Initialize the computer board
 *  0 means empty space with no ship
 *  1 means ship placed but not sunken
 *  2 means ship sunken
 *  3 means empty space chosen
*/
for (int i = 0; i < 16; i++) {
  computer_board[i] = 0;
}
int player_board[16]; // Same case for the player board
for (int i = 0; i < 16; i++) {
  player_board[i] = 0;
}
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
    if (digitalRead(arr[i]) == HIGH) {
      if (i < 4) {
        num += (i+1);
      } else {
        num += (i-4)*4
      }
    }
  }
  return num;
}
void setup() {
  // put your setup code here, to run once:
  pinMode(player_turn, OUTPUT);
  digitalWrite(player_turn, LOW);
  for (int i = 0; i < 8; i++) {
    pinMode(d_pin[i], INPUT);
  }
  for (int i = 0; i < 8; i++) {
    pinMode(d_ship_remain[i], OUTPUT);
    digitalWrite(d_ship_remain[i], LOW);
  }
  generate(primitive[]);
  for (int i = 0; i < 4; i++) {
    int buff = random(1,5);
    computer_board[primitive[i*4 + buff]] = 1;
  }
  digitalWrite(player_turn, HIGH);
  for (int i = 0; i < 8; i++) {
    digitalWrite(d_ship_remain[i], HIGH);
  }
  int count = 0;
  int light_off = 11;
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
    int select = translation(d_pin[]);
    player_board[select] = 1;
    for (int i = 0; i < 16; i++) {
      ship += player_board[i];
    }
    if (ship > count) {
      count++;
      digitalWrite(d_ship_remain[light_off + count], LOW);
    }
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
