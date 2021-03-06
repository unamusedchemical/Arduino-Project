#include <LiquidCrystal.h>

const int in = A0; // input pin for reading the sensor output
const int fan = 6; // output pin with PWM

const int rs = 12; // register select - command register when high, data register when low 
const int en = 11; // enable - send data to pins when a high to low pulse is given

// Data pins - send data to display
const int d4 = 5; // Data pin 4 
const int d5 = 4 ; // Data pin 5
const int d6 = 3; // Data pin 6
const int d7 = 2; // Data pin 7

LiquidCrystal lcd(rs, en, d4, d5, d6, d7); // create a variable of type LiquidCrystal

// Initialize custom characters for the first frame of the spinning fan 'animation' on the lcd
byte top[] = {
  B00000,
  B00000,
  B00000,
  B00011,
  B00111,
  B00111,
  B00111,
  B00011
};

byte right[] = {
  B11111,
  B11111,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};

byte left[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B01110,
  B11111,
  B11111
};

byte bottom[] = {
  B11000,
  B11100,
  B11100,
  B11100,
  B11000,
  B00000,
  B00000,
  B00000
};


// Initialize custom characters for the first frame of the spinning fan 'animation' on the lcd
byte top2[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B01000,
  B11100,
  B11110,
  B01111
};

byte right2[] = {
  B00001,
  B00011,
  B00111,
  B01111,
  B00110,
  B00000,
  B00000,
  B00000
};

byte left2[] = {
  B00000,
  B00000,
  B00000,
  B01100,
  B11110,
  B11100,
  B11000,
  B10000
};

byte bottom2[] = {
  B11110,
  B01111,
  B00111,
  B00010,
  B00000,
  B00000,
  B00000,
  B00000
};

void createFrames() {
  // frame one
  lcd.createChar(0, top);
  lcd.createChar(1, bottom);
  lcd.createChar(2, left);
  lcd.createChar(3, right);

  //frame two
  lcd.createChar(4, top2);
  lcd.createChar(5, bottom2);
  lcd.createChar(6, left2);
  lcd.createChar(7, right2);
}

void setup() {
  lcd.begin(16, 2);         // initializes the interface to the lcd; specifies the dimensions (width and height)
  createFrames();           // create custom icon
  analogReference(DEFAULT); // configures the reference voltage for the analog input; Default is 5V
}

void loop() {
  int ta = get_temp(); // we read the temperature from the sensor

  int percentage = map(ta, 25, 50, 0, 100);    // we calculate how fast the fan is spinning in percentage
  
  if(percentage > 100)
    percentage = 100;
  else if(percentage < 0)
    percentage = 0;

  // we print the temperature and the fan speed in percents
  displayFanSpeed(ta, percentage);

  analogWrite(fan, map(percentage, 0, 100, 0, 255)); // we turn the percentage value into a value between 0 and 255 to give to the pwm on pin 9  
  
  displayFrameOne(); // displays the first fan frame
  delay(400);

  if(percentage > 0) {
    displayFrameTwo(); // if the fan is spinning desplays the second frame after a delay indicated by the variable 'refresh' in ms
    delay(400);    
  }
}

void displayFanSpeed(double ta, int percentage){
  lcd.setCursor(2,0);
  lcd.print("Temp: ");
  lcd.print(ta);
  lcd.write(223); // degree symbol of the lcd charset
  lcd.print("C");
  lcd.setCursor(2,1);
  lcd.print("Speed: ");
  lcd.print(String(percentage) + "%"); 
}

double get_temp(){
  int ADCval = analogRead(in);
  return (25 * ADCval - 2050)/100;
}

void displayFrameOne() {
  lcd.setCursor(0,0);
  lcd.write(byte(0));
  lcd.setCursor(0,1);
  lcd.write(byte(3));
  lcd.setCursor(1,0);
  lcd.write(byte(2));
  lcd.setCursor(1,1);
  lcd.write(byte(1));
}

void displayFrameTwo() {
  lcd.setCursor(0,0);
  lcd.write(byte(4));
  lcd.setCursor(0,1);
  lcd.write(byte(7));
  lcd.setCursor(1,0);
  lcd.write(byte(6));
  lcd.setCursor(1,1);
  lcd.write(byte(5));
}