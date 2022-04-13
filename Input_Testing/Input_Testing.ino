void setup() {
  // put your setup code here, to run once:
  pinMode(30, INPUT_PULLUP);
  pinMode(31, INPUT_PULLUP);
  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
  pinMode(37, INPUT_PULLUP);
  pinMode(38, INPUT_PULLUP);
  Serial.begin(9600);
}

int readInput(int pin) {
  if (digitalRead(pin) == HIGH) {
    return 0;
  } else {
    return 1;
  }
}

int checker = 30;
int button[8];
void loop() {
  // put your main code here, to run repeatedly:
  while (digitalRead(30) == HIGH) {
    delay(1000);
    Serial.print("No Input \n");
  }
  while (digitalRead(30) == LOW) {
    button[0] = readInput(31);
    Serial.print(button[0]);
    button[1] = readInput(32);
    button[2] = readInput(33);
    button[3] = readInput(34);
    button[4] = readInput(35);
    button[5] = readInput(36);
    button[6] = readInput(37);
    button[7] = readInput(38);
    Serial.print("Input Detected \n");
  }
  for (int i = 0; i < 8; i++) {
    Serial.print(button[i]);
  }
  Serial.print("\n");
}
