void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);

   analogWrite(5, 0);
  analogWrite(6, 0);
}

void loop() {
  // put your main code here, to run repeatedly:


  
    char readIn = Serial.read();

    if(readIn == '2') {
      analogWrite(5, 100);
      analogWrite(6, 0);
    }
    
    else if(readIn == '3') {
      analogWrite(5, 0);
      analogWrite(6, 100);
    }
    else if(readIn == '5'){
      analogWrite(5, 0);
      analogWrite(6, 0);
    }
    


    delay(10);

}
