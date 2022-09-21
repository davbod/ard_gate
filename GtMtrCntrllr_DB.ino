/*
 * Gate Motor Controller:
 * 
 * 12vdc motor is connected to relay3-NO and relay2-com.
 * 12v battery is connected positive to relay1-NC and relay2-NO
 * 12v battery is connected negative to relay1-NO and relay2-NC
 * relay3-com is conntected to relay1-com
 * mains connected to relay 4 and courtesy light
 * 
 * Therefore relay 1 and 2 act together to change direction of the motor polarity and direction
 * Relay 3 acts as the switch to deliver power to the motor
 * 
 * relay 4 is a floodlight that should remain on for a few seconds after operation.
 * 
 * By David Bodmer
 */

const int relay1 = 2;
const int relay2 = 4;
const int relay3 = 7;
const int relay4 = 8;
const int button1 = 12;
const int beam = 13;
const int limitopen = 10;
const int limitclosed = 11;
const long interval = 20000;

unsigned long previousMillis = 0;

char *states[] = {"closed", "opening", "open", "closing", "stopped closed", "stopped open"};  
byte state = 0;

void setup() {
  Serial.begin(9600);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(button1, INPUT_PULLUP);
  pinMode(beam, INPUT_PULLUP);
  pinMode(limitopen, INPUT_PULLUP);
  pinMode(limitclosed, INPUT_PULLUP);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}

void loop() {
  unsigned long currentMillis = millis();
  int button1State = digitalRead(button1);
  int beamstate = digitalRead(beam);
  int limitopenstate = digitalRead(limitopen);
  int limitclosedstate = digitalRead(limitclosed);
  
  if (button1State == LOW && (states[state] == "closed" || states[state] == "stopped closed")) { //button opens gate
    state = 1; //opening
    digitalWrite(relay1, LOW); //relay 1 on
    digitalWrite(relay2, LOW); //relay 2 on 
    digitalWrite(relay3, LOW); //relay 3 on motor powered
    digitalWrite(relay4, LOW); //relay 4 on light is on
    Serial.println("statement 1 activated - button opens gate"); //verbose feedback
    delay(500);
    goto output;
  }
  else if (button1State == LOW && states[state] == "opening") { //button stops gate opening
    state = 5; //stopped open
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    Serial.println("statement 2 activated - button stops gate opening"); //verbose feedback
    delay(500);
    goto output;
  }
  else if (button1State == LOW && (states[state] == "stopped open" || states[state] == "open")) { //button closes gate
    state = 3; //closing
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    Serial.println("statement 3 activated - button closes gate"); //verbose feedback
    delay(500);
    goto output;
  }
  else if (button1State == LOW && states[state] == "closing") { //button stops gate closing
    state = 4; //stopped closed
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, LOW);
    Serial.println("statement 4 activated - button stops gate closing"); //verbose feedback
    delay(500);
    goto output;
  }
  else {
    delay(1);
  }

  delay(1);

  return;
  output:
  Serial.print(" Gate: ");
  Serial.println(states[state]);
  //Serial.print(" Light: ");
  //Serial.print(digitalRead(relay4));
  //Serial.println();
}
