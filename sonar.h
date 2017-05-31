#define LED PC5
#define LED1 PC4
#define LED2 PC3
#define TRIG PD1
#define TRIG1 PD4
#define TRIG2 PC1 
#define ECHO PD2 //do int0
#define ECHO1 PD3 //do int1
#define ECHO2 PB0 // do pcint0
unsigned int left1=0;
unsigned int left2=0;
unsigned int right1=0;
unsigned int right2=0;
unsigned int center1=0;
unsigned int center2=0;
unsigned int timer1overflow=0;
unsigned int flaga=0;
unsigned int flaga1=0;

extern double distance_left;
extern double distance_right;
extern double distance_center;

void init_pins();	//inicjalizuj potrzebne piny + timer
inline static void measure_distance();	//zmierz dystans, wyniki aktualizowane w zmiennych distance
void trigger();		//uruchom pomiar na pierwszym sonarze
void trigger1();	//uruchom pomiar na drugim sonarze
void trigger2();	//uruchom pomiar na trzecim sonarze