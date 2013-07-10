#ifndef __NOTES_
#define __NOTES_

const unsigned long SECOND = 1000000;
const unsigned long TIME[] ={36364,34323,32396,30578,28862,27242,25713,24270,22908,21622,20408,19263,18182,17161,16198,15289,14431,
13621,12856,12135,11454,10811,10204,9631,9091,8581,8099,7645,7215,6810,6428,6067,5727,5405,5102,4816,4545,4290,4050,3822,
3608,3405,3214,3034,2863,2703,2551,2408,2273,2145,2025,1911,1804,1703,1607,1517,1432,1351,1276,1204,1136,1073,1012,956,902,
851,804,758,716,676,638,602,568,536,506,478,451,426,402,379,358,338,319,301,284,268,253,239};

struct note{
  int n;
  int duty;
  unsigned long len;
  note* next;
};
void clearNotes();
void initSpeaker(const unsigned int pin);
void addNote(const int n, const unsigned long len, const int duty);
void insertNote(const int n, const unsigned long len, const int duty);
void advanceNote(const unsigned long ct);
void setSpeaker(const int state);
void playTone(const unsigned long ct);
void playMusic(const unsigned long ct);
bool isQuite();
void a(int octave, int sharp, int len);
void b(int octave, int sharp, int len);
void c(int octave, int sharp, int len);
void d(int octave, int sharp, int len);
void e(int octave, int sharp, int len);
void f(int octave, int sharp, int len);
void g(int octave, int sharp, int len);
#endif  
