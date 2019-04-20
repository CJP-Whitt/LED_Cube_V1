#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Dice_Animations.h>

// Used for software SPI
#define LIS3DH_CLK 13
#define LIS3DH_MISO 12
#define LIS3DH_MOSI 11
// Used for hardware & software SPI
#define LIS3DH_CS 10

// I2C
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
//   #define Serial SerialUSB
#endif

// Adjust this number for the sensitivity of the 'click' force
// this strongly depend on the range! for 16G, try 5-10
// for 8G, try 10-20. for 4G try 20-40. for 2G try 40-80
#define CLICKTHRESHHOLD 40

//Initial declarations
int check = 0; //For wait tap commands
int checkRef = 0; //For wait tap commands
int count = 0; //For general count usage
int interruptPin = 10; //For interrupt commands

void setup(){
  // put your setup code here, to run once:
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  strip.setBrightness(100);

  //#ifndef ESP8266
    //while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
  //#endif

  Serial.println("Adafruit LIS3DH Tap Test!");

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("LIS3DH found!");

  lis.setRange(LIS3DH_RANGE_4_G);   // 2, 4, 8 or 16 G!

  Serial.print("Range = "); Serial.print(2 << lis.getRange());
  Serial.println("G");

  // 0 = turn off click detection & interrupt
  // 1 = single click only interrupt output
  // 2 = double click only interrupt output, detect single click
  // Adjust threshhold, higher numbers are less sensitive
  lis.setClick(2, CLICKTHRESHHOLD);
  delay(100);

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), nextCommand, HIGH);

}

//******************************Void Loop*****************************************
//Defualt light demo function
void loop() {
  if(check !=checkRef)
    count++;

  switch (count){
    case 0: checkRef = check; 
            setAll(0,0,0);
            break;
    case 1: checkRef = check; 
            setAll(255,255,255);
            break;
    case 2: checkRef = check; 
            setAll(255,0,0);
            break;
    case 3: checkRef = check;
            setAll(0,255,0);
            break;
    case 4: checkRef = check;
            setAll(0,0,255);
            break;
    case 5: checkRef = check;
            setAll(255,0,255);
            break;
    case 6: checkRef = check;
            setAll(255,255,0);
            break;
    case 7: checkRef = check;
            setAll(0,255,255);
            break;
    case 8: checkRef = check;
            rainbow(40);
            break;
    case 9: checkRef = check;
            rainbowCycle(40);
            break;
    case 10:checkRef = check;
            RunningWheel();
            break;
    case 11:checkRef = check;
            BreathingLightRandom(25);
            break;
    case 12:checkRef = check;
            waterfall(25, 255, 0, 255);
            break;  
    case 13:checkRef = check;
            count=0;
            break;
   
  }
  strip.show();
} 

//*****************************End Void Loop**************************************

//*****************************Interrupt******************************************
void nextCommand(){
  check++;
  }
 
//*****************************End Interrupt*************************************

//*****************************Start Function Defontions*************************

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
    if(check !=checkRef)
        return;
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
    if(check !=checkRef)
        return;   
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
//Color wheel that spins on top and bottom as well as around the sides
void RunningWheel() {

      //set middle pixels on top/bot never change
      int count = 0;
      //Loop for wheels
      for(int i=0; i<8; i++){

        strip.setPixelColor(dice[3][1][1], 255, 255, 255);
        strip.setPixelColor(dice[1][1][1], 255, 255, 255);

        strip.setPixelColor(wheel[0][i], 255, 0, 0);
        strip.setPixelColor(wheel[1][i], 255, 0, 0);
        
          strip.setPixelColor(wheel[0][i+2], 0, 255, 0);
          strip.setPixelColor(wheel[1][i+2], 0, 255, 0);
         
           strip.setPixelColor(wheel[0][i+4], 0, 0, 255);
           strip.setPixelColor(wheel[1][i+4], 0, 0, 255);
          
            strip.setPixelColor(wheel[0][i+6], 255, 255, 0);
            strip.setPixelColor(wheel[1][i+6], 255, 255, 0);

            if(count%3 == 0){
               for(int n=0; n<3; n++){
                strip.setPixelColor(column[count][n], 255, 0, 0);
                strip.setPixelColor(column[count+3][n], 0, 255, 0);
                strip.setPixelColor(column[count+6][n], 0, 0, 255);
                strip.setPixelColor(column[count+9][n], 255, 255, 0);
               }
               count = count + 2;          
              }
               else{
                 for(int n=0; n<3; n++){
                    strip.setPixelColor(column[count][n], 255, 0, 0);
                    strip.setPixelColor(column[count-1][n], 255, 0, 0);
                    strip.setPixelColor(column[count+3][n], 0, 255, 0);
                    strip.setPixelColor(column[count+3-1][n], 0, 255, 0);
                    strip.setPixelColor(column[count+6][n], 0, 0, 255);
                    strip.setPixelColor(column[count+6-1][n], 0, 0, 255);
                    strip.setPixelColor(column[count+9][n], 255, 255, 0);
                    strip.setPixelColor(column[count+9-1][n], 255, 255, 0);
                 } 
                 count = count + 1; 
                }
                          
            strip.show();
            if(check !=checkRef)
              return;
            delay(300);
            //Turn all top and bottom off
            for(int i=0;i<8;i++){
            strip.setPixelColor(wheel[0][i], 0, 0, 0);
            strip.setPixelColor(wheel[1][i], 0, 0, 0);
            }
            //Turn all sides off
            for(int j=0; j<12; j++){
              for(int k=0; k<3; k++){
                strip.setPixelColor(column[j][k], 0, 0, 0);
              }
            }
      }
}

//For Normal Dice any color 
void StaticDice(int r, int g, int b) {
  for(int i=0; i<6; i++){ 
      Dice(i, r, g, b);
  }
  strip.show();
}

//Makes Full cube breathe desired color
//BreatheSpeed is the length of delay between inc/dec brightness
void BreathingLight(int breatheSpeed, int r, int g, int b){
      for(int j=0; j<3; j++){
          for(int k=0; k<3; k++){
            strip.setPixelColor(dice[0][j][k], r, g, b);
            strip.setPixelColor(dice[1][j][k], r, g, b);
            strip.setPixelColor(dice[2][j][k], r, g, b);
            strip.setPixelColor(dice[3][j][k], r, g, b);
            strip.setPixelColor(dice[4][j][k], r, g, b);
            strip.setPixelColor(dice[5][j][k], r, g, b);
           
          }
       }
       Serial.println("Breathe Bright");
       //Get brighter
       for(int i=5; i<100; i++){
        strip.setBrightness(i);
        strip.show();
        Serial.println("Brighter");
         if(i<50)
         delay(breatheSpeed * 3);
         else
         delay(breatheSpeed);
       }
       Serial.println("Breathe dark");
       //Get darker
       for(int i=100; i>5; i--){
       strip.setBrightness(i);
       strip.show();
       Serial.println("Darker");
       if(i<50)
       delay(breatheSpeed * 3);
       else
       delay(breatheSpeed);
       }
 }

//Makes cube breathe random colors 
void BreathingLightRandom(int breatheSpeed){
      int r = random(0,255);
      Serial.println(r);
      int g = random(0,255);
      Serial.println(g);
      int b = random(0,255);
      Serial.println(b);
      
      
      for(int j=0; j<3; j++){
          for(int k=0; k<3; k++){
            strip.setPixelColor(dice[0][j][k], r, g, b);
            strip.setPixelColor(dice[1][j][k], r, g, b);
            strip.setPixelColor(dice[2][j][k], r, g, b);
            strip.setPixelColor(dice[3][j][k], r, g, b);
            strip.setPixelColor(dice[4][j][k], r, g, b);
            strip.setPixelColor(dice[5][j][k], r, g, b);
           
          }
       }
       Serial.println("Breathe Bright");
       //Get brighter
       for(int i=5; i<100; i++){
        strip.setBrightness(i);
        strip.show();
        if(check !=checkRef)
          return;
        Serial.println("Brighter");
         if(i<50)
         delay(breatheSpeed * 3);
         else
         delay(breatheSpeed);
       }
       Serial.println("Breathe dark");
       //Get darker
       for(int i=100; i>5; i--){
       strip.setBrightness(i);
       strip.show();
       if(check !=checkRef)
         return;
       Serial.println("Darker");
       if(i<50)
       delay(breatheSpeed * 3);
       else
       delay(breatheSpeed);
       }
 }
void waterfall(int fallDelay, int r, int g, int b){
  setAll(0,0,0);
  strip.setBrightness(100);
  for(double p=0; p<=2.8; p=p+.01){
    if(p>0 && p<=1)
    strip.setPixelColor(dice[3][1][1], r*p, g*p, b*p); 
    if(p>1 && p<=2){
      strip.setPixelColor(dice[3][1][1], r*(2-p), g*(2-p), b*(2-p));
      strip.show();
    }
    if(check !=checkRef)
       return;
    strip.show();
    delay(fallDelay);
     
    if(p>.2 && p<=1.2){
      for(int i=0; i<8; i++){
        strip.setPixelColor(wheel[0][i], r*(p-.2), g*(p-.2), b*(p-.2));
      }
    }
    if(p>1.2 && p<=2.2){
        for(int i=0; i<8; i++){
        strip.setPixelColor(wheel[0][i], r*(2.2-p), g*(2.2-p), b*(2.2-p));
        }
      }
      strip.show();
    
    if(p>.4 && p<=1.4){
      for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][0], r*(p-.4), g*(p-.4), b*(p-.4));
      }
    }
    if(p>1.4 && p<=2.4){
        for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][0], r*(2.4-p), g*(2.4-p), b*(2.4-p));
        }
      } 
    strip.show();
    
    if(p>.6 && p<=1.6){
      for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][1], r*(p-.6), g*(p-.6), b*(p-.6));
      }
    }
    if(p>1.6 && p<=2.6){
        for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][1], r*(2.6-p), g*(2.6-p), b*(2.6-p));
        }
      }
     strip.show();
    
    if(p>.8 && p<=1.8){
     for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][2], r*(p-.8), g*(p-.8), b*(p-.8));
      }
    }
    if(p>1.8 && p<=2.8){
        for(int i=0; i<12; i++){
        strip.setPixelColor(column[i][2], r*(2.8-p), g*(2.8-p), b*(2.8-p));
        }
      }
     strip.show();
      if(check !=checkRef)
         return;
  }
}









          

    
