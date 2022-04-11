#define sensor A2 // Truck MAP sensor
#define user_switch 4 // Switch user pushes
#define redLED 6 // output for red LED, MOSFET needed 12VDC   
#define greenLED 2 // output for green LED, MOSFET needed 12VDC

// switch and debounce
bool state=true;
int buttonState;
int lastButtonState=LOW;
unsigned long lastDebounceTime=0;
unsigned long debounceDelay=30;

// vacuum from vap sensor
int vacuum=0; // raw sensor data
int inHG=0; // convert to inHG

int sensor_hreshold=4; // min vacuum required to switch to trigger state change

void setup() {
  Serial.begin(9600); // used for feeding data back to serial monitor
  pinMode(sensor,INPUT); // analog sensor reading in data
  pinMode(user_switch,INPUT_PULLUP); // switch that the user presses
  pinMode(redLED,OUTPUT); // set pin with red LED to output
  pinMode(greenLED,OUTPUT); // set pin with green LED to output
  digitalWrite(greenLED,LOW); // ensure green starts in off state
  digitalWrite(redLED,LOW); // ensure red starts in off state

}

void loop() {
  
  // switch debouncing circuit
  int reading=digitalRead(user_switch);
  if(reading!=lastButtonState)  {
    lastDebounceTime=millis();
  }
  if((millis()-lastDebounceTime)>debounceDelay)  {
    if(reading!=buttonState) {
      buttonState=reading;
      if(buttonState==LOW) {
        state = !state;
      }
    }
  }

  // switch logic, light changes based on button press
  if(state==true) {
    digitalWrite(greenLED,LOW);
    Serial.println("Switch OFF");
  }
  else if(state==false)  {
    digitalWrite(greenLED,HIGH);
    Serial.println("switch ON");
  }
  lastButtonState=reading;

  // vacuum reading and logic seperate from switch logic
  vacuum=analogRead(sensor); // read raw sensor data
  inHG = -0.0902*vacuum+30.223; // equation determined by excel trend line for user value

  if(inHG>5)  {
    digitalWrite(redLED,HIGH);
    Serial.println("Vacuum greater than 5 inHG");
  }
  else  {
    digitalWrite(redLED,LOW);
    Serial.println("Vacuum less than 5 inHG");
  }
}
