#define MA1 PD5
#define MA2 PB7
#define MB1	PB2
#define MB2 PB1
void init_motors();		//inicjalizuj potrzebne piny + pwm
void motor_direction(char,int);	//'A','B' silniki, wybor kierunku
void motor_speed(char,unsigned int); // 'A','B' silniki, 0-255 