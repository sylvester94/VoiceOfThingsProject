#include <ZumoMotors.h>
#include <Pushbutton.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>

#define MELODY_LENGTH 95
#define LED_PIN 13

unsigned char currentIdx;
int order = 0; // byte received on the serial port

ZumoMotors motors;
ZumoBuzzer buzzer;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN,LOW);
  Serial.begin(9600);

  currentIdx = 0;
}

unsigned char note[MELODY_LENGTH] = 
{
  NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_E(5), SILENT_NOTE, NOTE_C(5), NOTE_E(5),
  NOTE_G(5), SILENT_NOTE, NOTE_G(4), SILENT_NOTE,

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  NOTE_C(5), NOTE_G(4), SILENT_NOTE, NOTE_E(4), NOTE_A(4), NOTE_B(4), NOTE_B_FLAT(4), NOTE_A(4), NOTE_G(4),
  NOTE_E(5), NOTE_G(5), NOTE_A(5), NOTE_F(5), NOTE_G(5), SILENT_NOTE, NOTE_E(5), NOTE_C(5), NOTE_D(5), NOTE_B(4),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_C(6), SILENT_NOTE, NOTE_C(6), SILENT_NOTE, NOTE_C(6),

  SILENT_NOTE, NOTE_G(5), NOTE_F_SHARP(5), NOTE_F(5), NOTE_D_SHARP(5), NOTE_E(5), SILENT_NOTE,
  NOTE_G_SHARP(4), NOTE_A(4), NOTE_C(5), SILENT_NOTE, NOTE_A(4), NOTE_C(5), NOTE_D(5),

  SILENT_NOTE, NOTE_E_FLAT(5), SILENT_NOTE, NOTE_D(5), NOTE_C(5)
};

unsigned int duration[MELODY_LENGTH] =
{
  100, 25, 125, 125, 125, 125, 125, 250, 250, 250, 250, 250,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  375, 125, 250, 375, 250, 250, 125, 250, 167, 167, 167, 250, 125, 125,
  125, 250, 125, 125, 375,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 125, 125, 125, 250, 125, 125, 200, 50, 100, 25, 500,

  250, 125, 125, 125, 250, 125, 125, 125, 125, 125, 125, 125, 125, 125,

  250, 250, 125, 375, 500
};

void forward(int startSpeed, int endSpeed, int duration)
{
  for(int speed = startSpeed;speed <= endSpeed;speed+=10)
  {
    motors.setLeftSpeed(speed);
    motors.setRightSpeed(speed);
    delay(duration/((endSpeed - startSpeed)/10));
  }
}

void backward(int startSpeed, int endSpeed, int duration)
{
  for(int speed = startSpeed;speed <= endSpeed;speed+=10)
  {
    motors.setLeftSpeed(-speed);
    motors.setRightSpeed(-speed);
    delay(duration/((endSpeed - startSpeed)/10));
  }
}

void stop(int startSpeed, int endSpeed, int duration)
{
  for(int speed = startSpeed;speed >= endSpeed;speed-=10)
  {
    motors.setLeftSpeed(speed);
    motors.setRightSpeed(speed);
    delay(duration/((startSpeed - endSpeed)/10));
  }
}

void turnLeft(int startSpeed, int endSpeed, int duration)
{
  for(int speed = startSpeed;speed < endSpeed;speed+=10)
  {
    motors.setLeftSpeed(-speed);
    motors.setRightSpeed(speed);
    delay(duration/((endSpeed - startSpeed)/10));
  }
}

void turnRight(int startSpeed, int endSpeed, int duration)
{
  for(int speed = startSpeed;speed < endSpeed;speed+=10)
  {
    motors.setLeftSpeed(speed);
    motors.setRightSpeed(-speed);
    delay(duration/((endSpeed - startSpeed)/10));
  }
}

void loop()
{
  if (Serial.available() > 0) 
  {
    order = Serial.read();
    Serial.println(order);
   
    if (order == 66) // B
    {  
      forward(0,400,300);
      stop(400,0,300);
    }
    if(order == 67)   // C
    { 
      backward(0,400,300);
      stop(400,0,300);
    }
    if(order == 68)  // D
    {
      turnRight(0,200,300);
      stop(200,0,300);
    }
    if(order == 69)  // E 
    {  
      turnLeft(0,200,300);
      stop(200,0,300);
    }
    if (order == 70)  // turn around - F
    {
      for(int speed = 0; speed <= 400 ; speed++)
      {
        motors.setLeftSpeed(speed);
        motors.setRightSpeed(-speed);
        delay(10);
      } 
      motors.setLeftSpeed(0);
      motors.setRightSpeed(0);
    } 
    if(order == 71)  //tango -G
    { for(int i=0;i<=5;i++)
      {
        forward(0,400,200);
        stop(400,0,200);
        forward(0,400,200);
        stop(400,0,200);
        backward(0,400,200);
        stop(400,0,200);
      }                     
    }
    if(order == 72)  // crazy H
    {
      for(int i=0;i<=10;i++)
      {
        turnLeft(50,250,200);
        turnRight(50,250,200);
      }
      stop(250,0,200);
    }
    if(order == 65)  // Mario dance    // A
    {
      for(;;)
      {
        if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
        {
          buzzer.playNote(note[currentIdx], duration[currentIdx], 15);
          if(duration[currentIdx] >= 100)
          {
            forward(0,200,40);
            stop(200,0,30);
          }
          currentIdx++;
        }else if(currentIdx >= MELODY_LENGTH)
        {
          buzzer.stopPlaying();
          currentIdx = 0;
          break;
        }
      }
      Serial.print("--Arduino received: ");
      Serial.println(order);
    }
    else  
    {
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
    }
  }
}
