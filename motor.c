void init_motors()
{
	DDRD |= (1 << DDD6);	//PD6 to output
	DDRB |=(1<<DDB3);		//PB3 to output
	OCR2A = 70;				//ustaw PWM dla A
	OCR0A=70;				//ustaw PWM dla B


	TCCR0A |= (1 << COM0A1);		//ustaw tryb none-inverting
	TCCR0A |= (1 << WGM01) | (1 << WGM00);		//ustaw tryb fast PWM
	TCCR0B |= (1 << CS01);		//ustaw prescaler na 8 i uruchom PWM

	//analogicznie jak wyzej:	
    TCCR2A |= (1 << COM2A1);
    TCCR2A |= (1 << WGM21) | (1 << WGM20);
    TCCR2B |= (1 << CS21);

	DDRB |= (1<<MA2) | (1<<MB1) | (1<<MB2);			//piny MA2,MB1,MB2 to output
	DDRD |= (1<<MA1);		//pin MA1 to output
	PORTB |=  (1<<MB1);		//ustaw stan wysoki na MB1
	PORTD |= (1<<MA1);		//analogicznie na MA1
	PORTB &= ~(1<<MA2);		//stan niski na MA2
	PORTB &= ~(1<<MB2);		//analogicznie na MB2
}
void motor_direction(char motor,int dir){
	if(motor=='A'){
		PORTD &= ~(1<<MA1);
		PORTB &= ~(1<<MA2);	//wyzeruj piny
		if(dir==-1){
			PORTB |= (1<<MA2);		//do tylu
		}else
		if(dir==1){
			PORTD |= (1<<MA1);		//do przodu
		}		
	}else{
		PORTB &= ~(1<<MB1);
		PORTB &= ~(1<<MB2);	//wyzeruj piny
		if(dir==-1){
			PORTB |= (1<<MB2);		//do tylu
		}else
		if(dir==1){
			PORTB |= (1<<MB1);		//do przodu
		}
	}
	
}
void motor_speed(char motor,unsigned int speed){
	if(motor=='A'){
		OCR0A=speed;	//ustaw do PWM zadaną prędkość (zakres 0-255)
	}else{
		OCR2A=speed;	//analogicznie dla drugiego silnika
	}
}