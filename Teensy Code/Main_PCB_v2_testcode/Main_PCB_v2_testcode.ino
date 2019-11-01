#define cell1 14
#define cell2 15
#define cell3 16
#define cell4 17

#define relay_all 7
#define relay_odrv 21

#define current_all 22
#define current_odrv 23

#define current_odrv_offset 187
#define current_all_offset 191



void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);

pinMode(relay_all, OUTPUT);
pinMode(relay_odrv, OUTPUT);
analogReadResolution(10);
}

void loop() {
  // put your main code here, to run repeatedly:

activate_all();
activate_odrv();

delay(50);

// Buttons and knows on the on board UI
Serial.print(digitalRead(3)); Serial.print("  ");
Serial.print(digitalRead(4));Serial.print("  ");
Serial.print(digitalRead(5));Serial.print("  ");
Serial.print(digitalRead(6));Serial.print("  ");
Serial.println(analogRead(20));
/*
 
// READING CELL VOLTAGES - NEED TO LINEARISE SO WE GET THE ACTUAL VOLTAGE OUT!!
Serial.print("Cell1: "); Serial.print(analogRead(cell1));
Serial.print("   Cell2: "); Serial.print(analogRead(cell2));
Serial.print("   Cell3: "); Serial.print(analogRead(cell3));
Serial.print("   Cell4: "); Serial.println(analogRead(cell4));

delay(200);

*/
}

float curr_all() {
  return (analogRead(current_all) - current_all_offset) * (3.3 / 1024) * (1/0.06);
}

float curr_odrv() {
  return (analogRead(current_odrv) - current_odrv_offset) * (3.3 / 1024) * (1/0.06);
}

void activate_all() {
  digitalWrite(relay_all, HIGH);
}

void deactivate_all() {
  digitalWrite(relay_all, LOW);
}

void activate_odrv() {
  digitalWrite(relay_odrv, HIGH);
}

void deactivate_odrv() {
  digitalWrite(relay_odrv, LOW);
}
