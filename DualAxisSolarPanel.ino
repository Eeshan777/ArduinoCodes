#include <Servo.h>
#include <Adafruit_LiquidCrystal.h>

// Servo setup
Servo horizontalServo;
Servo verticalServo;
int ldrleft = A0;   // LDR for left
int ldrright = A1;  // LDR for right
int ldrup = A2;     // LDR for up
int ldrdown = A3;   // LDR for down
int ldrleftvalue = 0;
int ldrrightvalue = 0;
int ldrupvalue = 0;
int ldrdownvalue = 0;
int hpos = 90;      // Initial horizontal position
int vpos = 90;      // Initial vertical position
int deadzone = 5;   // Deadzone for movement

// LCD setup
float solarcell = 0;
Adafruit_LiquidCrystal lcd(0);

void setup() {
    // Servo setup
    horizontalServo.attach(8);
    verticalServo.attach(9);
    horizontalServo.write(hpos);
    verticalServo.write(vpos);
    
    // LCD setup
    pinMode(A0, INPUT); // A0 is used for LDR and solar cell
    Serial.begin(9600);
    lcd.begin(16, 2);
}

void loop() {
    // Read LDR values
    ldrleftvalue = analogRead(ldrleft);
    ldrrightvalue = analogRead(ldrright);
    ldrupvalue = analogRead(ldrup);
    ldrdownvalue = analogRead(ldrdown);
    
    // Calculate differences for servo movement
    int hdiff = ldrleftvalue - ldrrightvalue;
    int vdiff = ldrupvalue - ldrdownvalue;

    // Horizontal Movement
    if (abs(hdiff) > deadzone) {
        if (hdiff > 0) {
            hpos += 1;
        } else {
            hpos -= 1;
        }
    }

    // Vertical Movement
    if (abs(vdiff) > deadzone) {
        if (vdiff > 0) {
            vpos += 1;
        } else {
            vpos -= 1;
        }
    }

    // Constrain servo positions
    hpos = constrain(hpos, 0, 180);
    vpos = constrain(vpos, 0, 180);

    // Update Servo Positions
    horizontalServo.write(hpos);
    verticalServo.write(vpos);

    // Read solar cell voltage
    solarcell = (analogRead(A0) * (5.001 / 1023.001)); // Assuming A0 is also used for solar cell
    Serial.println(solarcell);

    // Update LCD display
    lcd.setCursor(0, 0);
    lcd.print("Solar Cell Volt");
    lcd.setCursor(0, 1);
    lcd.print("Value=");
    char buffer[10];
    dtostrf(solarcell, 4, 2, buffer);
    lcd.setCursor(10, 1);
    lcd.print(buffer);

    delay(100); // Delay for stability
}