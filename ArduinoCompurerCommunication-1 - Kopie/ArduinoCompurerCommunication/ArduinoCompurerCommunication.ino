#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x3F for a 20 chars and 4 line display

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String commandString = "";       // a string to hold incoming data

boolean isConnected = false;     // initializing as not connected 

void setup() {

  Serial.begin(9600);         
  initDisplay();
}

void loop() {
  serialEvent();

  if (stringComplete)         //  ready to start 
  {
    stringComplete = false;
    getCommand();

    if (commandString.equals("STAR"))  // to start clear the screen 
    {
      lcd.clear();
    }
    if (commandString.equals("STOP"))   // restarting toe initial state with a "Ready to connect" message 
    {
      lcd.clear();
      lcd.print("Ready to connect");
    }
    else if (commandString.equals("TEXT"))  // when text is being sent to screen shows it 
    {
      String text = getTextToPrint();
      printText(text);
    }

    inputString = "";
  }

}

void initDisplay()   // initialazing the LCD 
{
  lcd.init();
  lcd.backlight();
  lcd.print("Ready to connect");
}

boolean getLedState()   // initialazing the Led 
{
  boolean state = false;
  if (inputString.substring(5, 7).equals("ON"))
  {
    state = true;
  } else
  {
    state = false;
  }
  return state;
}

void getCommand()
{
  if (inputString.length() > 0)
  {
    commandString = inputString.substring(1, 5);
  }
}



String getTextToPrint()
{
  String value = inputString.substring(5, inputString.length() - 2);
  return value;
}

void printText(String text)
{
 // lcd.clear();
  lcd.setCursor(0, 0);
  int txtLen = text.length();
  if (txtLen < 20)   // setting the cursor in case of a 1 line text 
  {
    lcd.print(text);
  }
  else if (txtLen < 40) // setting the cursor in case of a 2 line text 
  {
    lcd.print(text.substring(0, 20));
    lcd.setCursor(0, 1);
    lcd.print(text.substring(20, 40));
  }
  else if (txtLen < 60) // setting the cursor in case of a 3 line text 
  {
    lcd.print(text.substring(0, 20));
    lcd.setCursor(0, 1);
    lcd.print(text.substring(20, 40));
    lcd.setCursor(0,2);
    lcd.print(text.substring(40, 60));
    int strLen = 20-text.substring(40,60).length();
    for(int i=0;i<strLen;i++)
    lcd.print(" ");
  }
  else  // setting the cursor in case of a 4 line text 
  {
    lcd.print(text.substring(0, 20));
    lcd.setCursor(0, 1);
    lcd.print(text.substring(20, 40));
    lcd.setCursor(0,2);
    lcd.print(text.substring(40, 60));
    lcd.setCursor(0,3);
    lcd.print(text.substring(60, 80));
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
