#include <arduino.h>
#include "notes.h"

note *head = 0;
note *tail = 0;
unsigned int speakerPin = 52;
void clearNotes()
{
  for(note* cur = head; cur != 0; cur = cur->next)
    free(cur);
  head = tail = 0;
}
void namedNote(int start, int octave, int sharp, int len)
{
  addNote(start + octave * 12 + sharp, len, 50);
  addNote(-1, len / 10, 0);
}
void a(int octave, int sharp, int len)
{
  namedNote(0, octave, sharp, len);
}
void b(int octave, int sharp, int len)
{
  namedNote(2, octave, sharp, len);
}
void c(int octave, int sharp, int len)
{
  namedNote(-9, octave, sharp, len);
}
void d(int octave, int sharp, int len)
{
  namedNote(-7, octave, sharp, len);
}
void e(int octave, int sharp, int len)
{
  namedNote(-5, octave, sharp, len);
}
void f(int octave, int sharp, int len)
{
  namedNote(-4, octave, sharp, len);
}
void g(int octave, int sharp, int len)
{
  namedNote(-2, octave, sharp, len);
}

bool isQuite()
{
  return head == 0;
}
void initSpeaker(const unsigned int pin)
{
  speakerPin = pin;
  pinMode(speakerPin, OUTPUT);
}
note* makeNote(const int n, const unsigned long len, const int duty, note* next)
{
    note *cur = (note*)malloc(sizeof(note));
    cur->n = n;
    cur->len = len*1000;
    cur->duty = duty;
    cur->next = next;
    return cur;
}  
void insertNote(const int n, const unsigned long len, const int duty)
{
  if(head == 0)
  {
    addNote(n, len, duty);
  }
  else
  {
    note* next = head->next;
    note *cur = makeNote(n, len, duty, head);
    head = cur;
  }
}
void addNote(const int n, const unsigned long len, const int duty)
{
  note *cur = makeNote(n, len, duty, 0);
  if(head == 0)
  {
    head = cur;
  }
  else
  {
    tail->next = cur;
  }
  tail = cur;
}

void advanceNote(const unsigned long ct)
{
  static unsigned long nt = ct;
  if(head == 0)
  {
    nt = ct;
  }
  else if(ct - nt > head->len)
  {
    note *old = head;
    head = head->next;
    free(old);
    nt = ct;
  }
}

void setSpeaker(const int state)
{
  static int state_ = LOW;
  if(state != state_)
  {
    digitalWrite(speakerPin, state);
    state_ = state;
  }
}

void playTone(const unsigned long ct)
{    
  static unsigned long lt = 0;
  if(-1 < head->n && head->n < 88) // in range
  {
    setSpeaker((ct - lt) * 100 < TIME[head->n] * head->duty);
    if(ct - lt >= TIME[head->n])
    {
      lt = ct;
    }
  }
  else
  {
    setSpeaker(LOW);
  }
}

void playMusic(const unsigned long ct)
{
  advanceNote(ct);
  if(head != 0)
  {    
    playTone(ct);
  }
}
