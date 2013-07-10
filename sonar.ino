#include "notes.h"

enum DIRECTIONS{  UP,  DOWN,  LEFT,  RIGHT, BUTTON_A, BUTTON_B };
const int FIRST_DIR = UP;
const int LAST_DIR = BUTTON_B;
const int INPUT_PINS[] = {2, 3, 4, 5, 6, 7};
const int GREEN = 49;
const int RED = 47;

unsigned long t = 20000;
unsigned int obj_x, obj_y, car_x, car_y;
bool gameStart;

void doInput(const unsigned long ct);
void ping(const unsigned long ct);
void crash(const int len);
void fanfare();
void russianAnthem();
void zap();
void wrong();
int distance();

int isUp()   { return !digitalRead(INPUT_PINS[UP]);      }
int isDown() { return !digitalRead(INPUT_PINS[DOWN]);    }
int isLeft() { return !digitalRead(INPUT_PINS[LEFT]);    }
int isRight(){ return !digitalRead(INPUT_PINS[RIGHT]);   }
int isA()    { return !digitalRead(INPUT_PINS[BUTTON_A]);}
int isB()    { return !digitalRead(INPUT_PINS[BUTTON_B]);}

void setup()
{
  randomSeed(analogRead(15));
  car_x = car_y = 24;
  pinMode(51, OUTPUT);
  initSpeaker(53);
  for(int pin = FIRST_DIR; pin <= LAST_DIR; ++pin)
  {
    pinMode(INPUT_PINS[pin], INPUT);
    digitalWrite(INPUT_PINS[pin], HIGH);
  }
  gameStart = false;
  
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  OCR1A = 0xffff;
  TCCR1B |= (1 << CS11 | 1 << CS10);
  interrupts();
//  fanfare();
  russianAnthem();
}
void fanfare()
{
  for(int i = 0; i < 10; ++i)
  {
    for(int o = 5; o <= 8; ++o)
    {
      c(o, 0, 250);
    }
  }
}
void russianAnthem()
{
  g(3, 0, 250);
  c(4, 0, 500);
  g(3, 0, 375);
  a(3, 0, 125);
  b(3, 0, 500);
  e(3, 0, 250);
  e(3, 0, 250);
  a(3, 0, 500);
  g(3, 0, 375);
  f(3, 0, 125);
  g(3, 0, 500);
  c(3, 0, 250);
  c(3, 0, 250);
  d(3, 0, 500);  
  d(3, 0, 375);  
  e(3, 0, 125);  
  f(3, 0, 500);  
  f(3, 0, 375);  
  g(3, 0, 125);  
  a(3, 0, 500);  
  b(3, 0, 250);  
  c(4, 1, 250);  
  d(4, 1, 750);
}

void loop()
{
  static unsigned long lt = 0;
  static int state = 0;
  unsigned long ct = micros();
  playMusic(ct);
  if(gameStart)
  {
    digitalWrite(RED, car_x < obj_x);
    digitalWrite(GREEN, car_y < obj_y);
    doInput(ct);
    if(ct - lt > t)
    {
      digitalWrite(51, state);
      state = 1 - state;
      lt = ct;
    }
  }
  else
  {
    gameStart = isQuite();
    obj_x = random(0, 50);
    obj_y = random(0, 50);
  }
}
void doInput(const unsigned long ct)
{
  static unsigned long it = 0;
  if((ct - it) * 10 > 2 * SECOND)
  {
    if(isUp() | isDown() | isLeft() | isRight())
    {
      t = (t > 10000) ? (t-2500) : t; it = ct;
    }
    else              { t = 20000; it = ct; }
    
    if(isUp() && car_y < 50)        { car_y += 1;   it = ct; }
    else if(isDown() && car_y > 0)  { car_y -= 1;   it = ct; }
    if(isLeft() && car_x > 0)       { car_x -= 1;   it = ct; }
    else if(isRight() && car_x < 50){ car_x += 1;   it = ct; }      
    
    if(isA())         { ping(ct); }
    
    if(isB())
    { 
      int dist = distance();
      if(dist < 8)
      {
        crash(250);
      }
      else
      {
        wrong();
      }
      zap(); it = ct;
      gameStart = false;
    }
  }
}
int distance()
{
  unsigned int dist_x = obj_x - car_x;
  unsigned int dist_y = obj_y - car_y;
  unsigned int dist_sq = dist_x * dist_x + dist_y * dist_y;
  return dist_sq;
}
void ping(const unsigned long ct)
{
  static unsigned long pt = 0;
  static unsigned long time_ = 0;
  
  unsigned long time = distance() >> 2;
  if(ct - pt > time_)
  {
    clearNotes();
    addNote(61, 125, 50);
    addNote(74, 125, 50);
    addNote(-1, time, 0);
    if(car_x < obj_x)
      addNote(48, 125, 50);
    else
      addNote(74, 125, 50);
    if(car_y < obj_y)
      addNote(48, 125, 50);    
    else
      addNote(74, 125, 50);
      
    pt = ct;
    time_ = (time + 750) * 1000;
  }
}

void zap()
{
  for(int i = 0; i < 10; ++i)
  {
    insertNote(52 + i*2, 10, 50);
  }
}

void crash(const int len)
{
  for(int i = 0; i < len; ++i)
  {
    insertNote(random(5, 22), 10, 25);
  }
}

void wrong()
{
  insertNote(-1, 500, 50);
  insertNote(19, 250, 50);
  insertNote(21, 250, 50);
  insertNote(23, 250, 50);
  insertNote(-1, 1000, 50);
}
