#include <LiquidCrystal.h>
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);//RS,EN,D4,D5,D6,D7
#include <Keypad.h>
const byte ROWS = 4;
const byte COLS = 4;
float p;
int array1[4] = { 1, 0, 0, 1};
int array2[4] = { 1, 1, 0, 0};
int counts = 8;

float minute;
String mnt;
String bls = "";
float bolus;


float pulse;
int j = 13;

char keys[ROWS][COLS] = {
  {'7', '8', '9', 'A'},
  {'4', '5', '6', 'B'},
  {'1', '2', '3', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {53, 51, 49, 47};

byte colPins[COLS] = {45, 43, 41, 39};
//  Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
void setup()
{
  pinMode(5, OUTPUT);

  // Set the outputs for Stepper motor
  for (int k = 8; k <= 9; k++ )
  {
    pinMode(k, OUTPUT);
  }

  lcd.begin(16, 2); //initializing LCD
}
void loop()
{
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("LENTA");
  lcd.setCursor(2, 1);
  lcd.print("SYRINGE PUMP");
  delay(1000);

  FluidSize();

}

void FluidSize()
{
  int a = 10;
  bolus = 0;
  bls = "";

  while (1)
  {

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FluidSize");
    while (1)
    {
      char key10 = kpd.getKey();
      if (key10 != NO_KEY)
      {

        lcd.setCursor(a + 2   , 0);
        lcd.print(key10);
        lcd.setCursor(14, 0);
        lcd.print("mL");
        bls += (char)key10;
        bolus = bls.toFloat();
        a++;
        if (key10 == '#') {
          key10 = NO_KEY;
          control();

        }
        if (key10 == '*') {
          key10 = NO_KEY;
          return FluidSize();

        }
      }
    }
  }
}


void minutes() {
  int m = 9;
  mnt = "";
  minute = 0;

  while (1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("FluidSize ="); lcd.print(bolus); lcd.print("mL");
    lcd.setCursor(0, 1);
    lcd.print("Time =");
    while (1) {
      char key11 = kpd.getKey();
      if (key11 != NO_KEY) {
        lcd.setCursor(m, 1);
        lcd.print(key11); lcd.print(" min");
        mnt += (char)key11;
        minute = mnt.toFloat();
        m++;
        if (key11 == '#') {
          key11 = NO_KEY;
          timeControl();
        }
        if (key11 == '*') {
          key11 = NO_KEY;
          return minutes();



        }

      }

    }
  }
}
void flowrate() {

  while (1)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Flow Rate =");
    lcd.setCursor(0, 1);
    lcd.print(float(bolus / minute)); lcd.print(" mL/min");
    delay(1000);
    lcd.clear();
    Dir();


  }
}

void Dir()
{
  while (1)
  {


    lcd.setCursor(0, 0);
    lcd.print("Direction");

    while (1)
    {
      char key = kpd.getKey(); //storing pressed key value in a char

      if (key != NO_KEY)
      {
        //For Left rotation
        if (key == 'C')

        {
          key = NO_KEY; // Clear the pressed key data
          while (1)
          {
            lcd.setCursor(12, 0);
            lcd.print("PULL");
            delay(100);
            lcd.setCursor(0, 1);
            pulse = ((60000 * minute) / (5760 * 2 * bolus));
            lcd.print("PRESS ENTER(=)");
            delay(100);
            while (1) {

              char key20 = kpd.getKey();
              if (key20 == '*') {
                key20 = NO_KEY;
                return flowrate();
              }
              if (key20 == '#')
              {
                while (1) {
                  for (int p = 5; p >= 0; p--) {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Starts in 5 Secs");
                    lcd.setCursor(0, 1);

                    lcd.print(p);
                    delay(500);

                  }
                  if (p == 0) {
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("Starting...");
                    delay(500);
                    lcd.clear();
                    motor_run('C');

                  }
                }


              }


            }
          }
        }
      }



      //For Right rotation
      if (key == 'D')

      {
        key = NO_KEY;
        while (1)
        {
          lcd.setCursor(12, 0);
          lcd.print("PUSH");
          delay(100);
          lcd.setCursor(0, 1);
          pulse = ((60000 * minute) / (5760 * 2 * bolus));
          lcd.print("PRESS ENTER(=)");

          delay(100);
          while (1) {
            char key20 = kpd.getKey();

            if (key20 == '*') {
              key20 = NO_KEY;
              return flowrate();

            }




            if (key20 == '#')
            {
              while (1) {
                for (int p = 5; p >= 0; p--) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Starts in 5 Secs");
                  lcd.setCursor(0, 1);

                  lcd.print(p);
                  delay(500);

                }
                if (p == 0) {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("Starting...");
                  delay(500);
                  lcd.clear();
                  motor_run('D');

                }
              }

            }

          }
        }
      }
    }
  }
}



void motor_run(char key)
{
  unsigned long endTime = millis() + 30000 * minute;
  while (millis() <= endTime)
  {

    if (key == 'D') {


      for (int i = 0; i < counts; i++) {

        digitalWrite(8, array1[(i % 4)]);
        digitalWrite(9, array2[(i % 4)]);
        delay(pulse);
      }
    }




    if (key == 'C') {

      for (int i = 0; i < counts; i++) {
        digitalWrite(8, array1[3 - (i % 4)]);
        digitalWrite(9, array2[3 - (i % 4)]);
        delay(pulse);
      }
    }


  } while (millis() == 0 ); {
    while (1) {
      lcd.setCursor(0, 0);
      lcd.print("Finished");

      lcd.setCursor(0, 1);
      lcd.print("Press Reset");
     
      digitalWrite(5, HIGH);
      delay(1000);
      digitalWrite(5, LOW);
      delay(1000);
    }


  }


}



void control()
{ while (1) {
    if (bolus <= 50 && bolus > 0) {


      minutes();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("WRONG VALUE");
      delay(500);
      FluidSize();
    }
  }
}

void timeControl()
{ while (1) {


    if (minute > 0 && minute <= 300) {


      flowrate();
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Value");
      lcd.setCursor(0, 1);
      lcd.print("Between 1-300");
      delay(1000);
      lcd.clear();
      minutes();

    }
  }
}
