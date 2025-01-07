#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Motor A connections
int enA = 11;
int in1 = 51;
int in2 = 53;

// Motor B connections./
int enB = 12; 
int in3 = 41;
int in4 = 43;

// IR Sensor to check Rotation 
int IRs = A2;
int IR = 0;

float circumference = 34.00;
int test_count = 0;

const int ROW_NUM = 4;   //four rows
const int COLUMN_NUM = 3;   //three columns

char keys[ROW_NUM][COLUMN_NUM] = 
{
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};   //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3};   //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );
LiquidCrystal_I2C lcd(0x3F, 16, 2);   // I2C address 0x27, 16 column and 2 rows

int cursorColumn = 0;
int cursorRow = 1;

float path1[] = {1.01, 2.0, 1.01, 2.0, 1.01, 2.0, 1.01, 9.0};
float path2[] = {1.02, 2.0, 1.02, 2.0, 1.02, 2.0, 1.02, 9.0};
float path3[] = {1.02, 2.0, 1.02, 3.0, 1.02, 9.0};
float path4[] = {1.01, -1.01, 2.0, 1.01, 9.0};
float path5[] = {1.01, 3.0, 1.01, 3.0, 1.01, 3.0, 1.01, 9.0};

void setup() {

  // Set all the motor control pins and IR inputs
  pinMode(A2, INPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.begin(9600);

  lcd.init(); // initialize the lcd
  lcd.backlight();
  lcd.print("  Welcome to ITM University");
  for (int positionCounter = 0; positionCounter < 26; positionCounter++)
  {
    lcd.scrollDisplayLeft();
    delay(500);
  } 
  lcd.clear();
  lcd.print("Enter Rack Num");
  
}

void loop() 
{
  char key = keypad.getKey();
  if(test_count < 1)
  {
    if(key) 
    {
      Serial.println(key);
      lcd.setCursor(cursorRow, 1); 
      lcd.print(key);                 
      if(cursorRow == 1) 
      {       
        lcd.clear();
        cursorRow = 0;
      }
      int alpha = key - '0';    //char to int conversion
      Serial.println(alpha);
      KeyCheck(alpha);
    }
  }
}

void KeyCheck(int key1)
{
  
  Serial.println(key1);
  while( key1 > 0 )
  {
    if(test_count < 1)
    {
      switch (key1) 
      {
          case 1:    
            Serial.println("Path1");
            test_count = test_count + 1;
            Trace_path(path1);
            break;

          case 2: 
            Serial.println("Path2"); 
            test_count = test_count + 1;
            Trace_path(path2);
            break;

          case 3: 
            Serial.println("Path3");  
            test_count = test_count + 1; 
            Trace_path(path3);
            break;

          case 4:
            Serial.println("Path4");
            test_count = test_count + 1;    
            Trace_path(path4);
            test_count = test_count + 1;
            break;

          case 5:
            Serial.println("Path5");
            test_count = test_count + 1;
            Trace_path(path5);
            break;

          default:
            Serial.println("No such path found.");
            break;
        }
    delay(1);     
    }
    else
    {
      break;
      Serial.println("BREAK");
    }
  }  
}


void Trace_path(float input[])
{
  for(int i=0; i < Farr_Size(input); i++)
  {
    float dist = 0;
    //Serial.println(input[i]);
    int tmp = (int)input[i];
    Serial.println("\nDirection Case ");
    Serial.println(tmp);
    switch(tmp)
    {
      case 1:
        dist = (input[i] - 1) * 100;
        Forward(dist);
        break;
        
      case -1:
        dist = (input[i] + 1) * 100;
        Backward(dist);
        break;

      case 2:
        Right_Mov();
        break;

      case 3:
        Left_Mov();
        break;

      default:
        Serial.println(" PATH ERROR ");
        break;
    }
  }
}

int Farr_Size(float arr[])
{
  int arrsize = 0;
  for(int i=0; arr[i] != 9.0; i++)
  {
    arrsize = arrsize + 1;  
  }
  return arrsize;
}

int Forward(float distm)
{
  float distc, rots;
  int chk = 0;
  distc = distm * 100;
  rots = distc / circumference;
  rots = round(rots);

  analogWrite(enA, 125);
  analogWrite(enB, 120);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

  Serial.println("Inside Forward");
  Check_Stop(rots);
}

int Left_Mov()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("Left");
  delay(950);
  Stop();
}

int Right_Mov()
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("Right");
  delay(950);
  Stop();
}

int Stop()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  Serial.println("Stop");
  delay(1000);
}

int Backward(float distm)
{

  float distc, rots;
  int chk = 0;
  distc = distm * 100;
  rots = distc / circumference;
  rots = -rots;
  rots = round(rots);
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 120);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 125);
  Serial.println("Inside Backward");
  Check_Stop(rots);

}

int Check_Stop( float r)
{
  float rot_count = 0.0;
  while( r > 0.0)
  {
    IR = analogRead(IRs);
    delay(175);
    if(IR < 500)
    {
      rot_count = rot_count + 1.0;
      //Serial.println(rot_count);
      if(rot_count == r)
      {
        Serial.print("\nEqual and Stop");
        Stop(); 
        r = 0.0;
      }
    }
  }
}
