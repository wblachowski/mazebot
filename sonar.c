double distance_left=0;
double distance_right=0;
double distance_center=0;
void init_pins(){
	DDRC |= (1<<LED) | (1<<LED1) | (1<<LED2);  //ledy output
	DDRD |=(1<<TRIG);	
	DDRD |= (1<<TRIG1);
	DDRC |=	(1<<TRIG2);	 //trig output
	DDRD &=~(1<<ECHO);	 //echo input
	DDRD &=~(1<<ECHO1);	 //echo1 input
	DDRB &=~(1<<ECHO2);  //echo2 iput
	PORTD |= ((1 << ECHO) | (1 << ECHO1));	//pull-up na wejściach
	PORTB |= (1<<ECHO2); //jak wyżej
	PORTC |= (1<<LED);		
	PORTC |= (1<<LED1);		
	PORTC |= (1<<LED2);	 //ustaw stan wysoki na ledach

	EIMSK|= ((1<<INT0) | (1<<INT1)); //wlacz przerwania na int0 i int1
	EICRA|=(1<<ISC00) | (1<<ISC01);	 //zbocze narastajace uruchomi int0
	EICRA|=(1<<ISC10) | (1<<ISC11);	 //zbocze narastajace uruchomi int1
	PCICR |= (1<<PCIE0);	//wlacz skanowanie rejestru PCMSK0 
	PCMSK0 |= (1<<PCINT0);  //wlacz przerwania na pinie PCINT0
	TIMSK1 |= (1<<TOIE1);   //zgoda na przerw. po przepełnieniu timera
    
	_delay_ms(2000);	//2 sekundy przed ruszeniem
}
void trigger(){
	timer1overflow=0;
	PORTD &= ~(1<<TRIG); // Ustaw stan niski
	_delay_us(1);        // Czas na ustawienie pinu
	PORTD |= (1<<TRIG);  // Ustaw stan wysoki (Zbocze narastające)
	_delay_us(12);       // Podtrzymaj stan wysoki przez 12us
	PORTD &= ~(1<<TRIG); // Ustaw znów stan niski
	_delay_us(1);        // Delay na wszelki wypadek
}
void trigger1(){
	PORTD &= ~(1<<TRIG1);           
	_delay_us(1);           
	PORTD |= (1<<TRIG1);    
	_delay_us(12);         
	PORTD &= ~(1<<TRIG1);       
	_delay_us(1);        // WSZYSTKO JAK WYŻEJ W trigger()
}
void trigger2(){
	PORTC &= ~(1<<TRIG2);          
	_delay_us(1);         
	PORTC |= (1<<TRIG2);      
	_delay_us(12);         
	PORTC &= ~(1<<TRIG2);     
	_delay_us(1);       // WSZYSTKO JAK WYŻEJ W trigger()
}
inline static void measure_distance(){
	EICRA=0x00;		//wyzeruj rejest EICRA
	//wlaczenie przerwan na INT0,INT1:
	EICRA|=((1<<ISC00) | (1<<ISC01) | (1<<ISC10) | (1<<ISC11));
	flaga=0;
	flaga1=0;
	
	TCNT1=0;		     //wyzeruj rejest timera
	TCCR1B |= (1<<CS00); //zacznij liczyc,zegar zewnetrzny
	
	trigger();		//wyslij zbocze na TRIGGER 1 czujnika
	_delay_ms(10);		
	trigger1();		//wyslij zbocze na TRIGGER 2 czujnika
	_delay_ms(10);
	trigger2();		//wyslij zbocze na TRIGGER 3 czujnika
	_delay_ms(40);	//odstep miedzy pomiarami 40+10+10
	
	TCCR1B=0x00;	//zatrzymaj timer
}
ISR(TIMER1_OVF_vect){
	timer1overflow++; //przepełnienie rejestru licznika
	TCNT1=0;		 //wyzeruj rejestr licznika
}
ISR(PCINT0_vect){
	if( (PINB & (1 << PINB0)) == 1 )
	{
		//zbocze narastajace wywołało przerwanie
		center1=timer1overflow*65536+TCNT1;	
	}
	else
	{
		//zbocze opadajace wywolalo przerwanie
		center2=timer1overflow*65536+TCNT1;	
		distance_center=((double)center2-(double)center1)/58.0;	
	}
}
ISR(INT0_vect){
	if(flaga==0){	//zbocze narastajace
		right1=timer1overflow*65536+TCNT1;
		EICRA &= ~(1<<ISC00);	//wyzeruj bit ISC00 
		//przerwanie wywola zbocze opadajace:
		EICRA |= (1<<ISC01);
		flaga=1;	
	}else{
		right2=timer1overflow*65536+TCNT1;	/
		distance_right=((double)right2-(double)right1)/58.0;
		//przerwanie wywola zbocze narastajace:
		EICRA |= (1<<ISC00) | (1<<ISC01);
		flaga=0;
	}
}
ISR(INT1_vect){
	//tu wszystko analogicznie jak w ISR(INT0_vect)
	if(flaga1==0){	//wlacz timer
		left1=timer1overflow*65536+TCNT1;
		EICRA &= ~(1<<ISC10);
		EICRA |= (1<<ISC11);  //zbocze opadajace uruchomi
		flaga1=1;
	}else{
		left2=timer1overflow*65536+TCNT1;
		distance_left=((double)left2-(double)left1)/58.0;
		//zbocze narastajace uruchomi:
		EICRA |= (1<<ISC00) | (1<<ISC01);
		flaga1=0;
	}
}