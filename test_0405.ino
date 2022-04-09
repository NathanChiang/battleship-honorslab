//const int input = 40;
void setup() {
  // put your setup code here, to run once:
  pinMode(40, INPUT);
 // pinMode(30, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  // found = digitalRead(input);
  int found = digitalRead(40);
  if(found == HIGH) {
    Serial.print("Here");
    delay(1000);
  }

  else{
    Serial.print("Found");
    
    delay(1000);
   
  
  }
}
