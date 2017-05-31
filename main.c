#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "sonar.h"
#include "sonar.c"
#include "motor.h"
#include "motor.c"
#define NORM 0.65	//predkosc w trybie szybkim (0-1)
#define SLOW 0.55	//predkosc w trybie wolnym (0-1)
int main(void)
{
	init_pins();
	init_motors();
	//ustawiamy jazde do przodu:
	motor_direction('A',1);	
	motor_direction('B',-1);	//-1, kabel podlaczony na odwrot
	//ustawiamy predko?? na 0:
	motor_speed('A',0);
	motor_speed('B',0);
	sei();	//globalna zgoda na przerwania
    while (1) 
    {			
		measure_distance();		//pomiar odleglosci
		motor_direction('A',1);
		motor_direction('B',-1);
			//TRZYMANIE SIE PRAWEJ SCIANY
			if(distance_right>=10 && distance_right<=12){
				if(distance_center>=40){
					motor_speed('A',245*NORM);
					motor_speed('B',255*NORM);}
				else{
					motor_speed('A',245*SLOW);
					motor_speed('B',255*SLOW);				
				}
				PORTC |= (1<<LED2);
			}else{PORTC &= ~(1<<LED2);}
			//OODAL SIE OD SCIANY:
			if(distance_right>=7 && distance_right<=10){
				if(distance_center>=40){
					motor_speed('A',225*NORM);
					motor_speed('B',255*NORM);
				}else{
					motor_speed('A',225*SLOW);
					motor_speed('B',255*SLOW);
				}
			}
			if(distance_right>=5 && distance_right<=7){
				if(distance_center>=40){
					motor_speed('A',200*NORM);
					motor_speed('B',255*NORM);	
				}else{
					motor_speed('A',200*SLOW);
					motor_speed('B',255*SLOW);	
				}
			}
			if(distance_right<=5){
				if(distance_center>=40){
					motor_speed('A',150*NORM);
					motor_speed('B',255*NORM);
				}else{
					motor_speed('A',150*SLOW);
					motor_speed('B',255*SLOW);					
				}
			}
			//PRZYBLIZ SIE DO SCIANY:
			if(distance_right>=12 && distance_right<=17)
			{
				if(distance_center>=40){
					motor_speed('A',245*NORM);
					motor_speed('B',220*NORM);
				}else{
					motor_speed('A',245*SLOW);
					motor_speed('B',220*SLOW);					
				}
			}
			if(distance_right>=17 && distance_right<=25)
			{
				if(distance_center>=40){
					motor_speed('A',245*NORM);
					motor_speed('B',200*NORM);
				}else{
					motor_speed('A',245*SLOW);
					motor_speed('B',200*SLOW);
				}
			}
			if(distance_right>=25)
			{
				motor_speed('A',165*NORM);	
				motor_speed('B',70*NORM);	
				PORTC|=(1<<LED);
			}else{
				PORTC&=~(1<<LED);	
			}

			//SKRET W LEWO ORAZ OBROT
			if(distance_center<=15 && distance_right<=15){
				motor_speed('A',0);
				motor_direction('A',-1);
				motor_speed('A',130);
				motor_speed('B',130);
				while(distance_center<40){
					measure_distance();
				}
				motor_speed('A',0);
				motor_speed('B',0);
			}
			if(distance_center<=15){
				PORTC&=~(1<<LED1);
			}else{
				PORTC|=(1<<LED1);
			}		
	}
return 0;
}

