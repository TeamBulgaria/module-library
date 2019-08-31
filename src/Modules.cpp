#include "Modules.h"
//class Motor
Motor::Motor(){
    pinMode(MOTOR_LEFT_FORWARD,OUTPUT);
    pinMode(MOTOR_LEFT_BACKWARD,OUTPUT);
    pinMode(MOTOR_RIGHT_BACKWARD,OUTPUT);
    pinMode(MOTOR_RIGHT_FORWARD,OUTPUT);
}

void Motor::leftRun(int8_t speed){
    _left_speed=speed;
    if(speed>100)speed=100;
    if(speed<-100)speed=-100;
    if(speed>0){
        analogWrite(MOTOR_LEFT_FORWARD,map(speed,1,100,LOW_SPEED,HIGH_SPEED));
        analogWrite(MOTOR_LEFT_BACKWARD,0);
    }else if(speed<0){
        analogWrite(MOTOR_LEFT_FORWARD,0);
        analogWrite(MOTOR_LEFT_BACKWARD,map(-speed,1,100,LOW_SPEED,HIGH_SPEED));
    }else{
        //speed == 0
        rightStop();
    }
}

void Motor::rightRun(int8_t speed){
    _right_speed=speed;
    if(speed>100)speed=100;
    if(speed<-100)speed=-100;
    if(speed>0){
        analogWrite(MOTOR_RIGHT_FORWARD,map(speed,1,100,LOW_SPEED,HIGH_SPEED));
        analogWrite(MOTOR_RIGHT_BACKWARD,0);
    }else if(speed<0){
        analogWrite(MOTOR_RIGHT_FORWARD,0);
        analogWrite(MOTOR_RIGHT_BACKWARD,map(-speed,1,100,LOW_SPEED,HIGH_SPEED));
    }else{
        //speed == 0
        rightStop();
    }
}

void Motor::leftStop(){
    _left_speed=0;
    analogWrite(MOTOR_LEFT_FORWARD,0);
    analogWrite(MOTOR_LEFT_BACKWARD,0);
}

void Motor::rightStop(){
    _right_speed=0;
    analogWrite(MOTOR_RIGHT_FORWARD,0);
    analogWrite(MOTOR_RIGHT_BACKWARD,0);
}

//class Ultrasound
Ultrasound::Ultrasound(){
    pinMode(ULTRASOUND_ECHO,INPUT);
    pinMode(ULTRASOUND_TRIG,OUTPUT);
}

double Ultrasound::getDistanceCM(){
    digitalWrite(ULTRASOUND_TRIG,LOW);
    delayMicroseconds(5);
    digitalWrite(ULTRASOUND_TRIG,HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASOUND_TRIG,LOW);

    double duration = pulseIn(ULTRASOUND_ECHO,HIGH);
    //29.1 us/cm in 20oC
    return 0.5/29.1*duration;
}

//class TurnSignal
ISR(TIMER1_COMPA_vect){
    TurnSignal::timer1ISR();
}

TurnSignal::TurnSignal(){
    pinMode(TURNSIGNAL_L,OUTPUT);
    pinMode(TURNSIGNAL_R,OUTPUT);
}

void TurnSignal::initBlink(unsigned int freq){
    //Init timer 1
    noInterrupts();
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    /* Time Compare Value: 16MHz/prescalar/2Hz/2 intervals=15625 */
    OCR1A = 16e+6/256/freq/2;      //time compare value
    TCCR1B |= (1 << WGM12);    /* CTC Mode */
    TCCR1B |= (1 << CS12);    /* 256 prescalar */
    TIMSK1 |= (1 << OCIE1A);    /* Enable timer compare interrupt */
    interrupts();
}

void TurnSignal::setLeft(bool state){
    _blink_left=false;
    digitalWrite(TURNSIGNAL_L,state);
}

void TurnSignal::setRight(bool state){
    _blink_right=false;
    digitalWrite(TURNSIGNAL_R,state);
}

void TurnSignal::timer1ISR(){
    if(_blink_right){
        digitalWrite(TURNSIGNAL_R,!digitalRead(TURNSIGNAL_R));
    }
    if(_blink_left){
        digitalWrite(TURNSIGNAL_L,!digitalRead(TURNSIGNAL_L));
    }
}

void TurnSignal::setBlinkLeft(bool state){
    _blink_left=state;
    if(!state)digitalWrite(TURNSIGNAL_L,LOW);
}

void TurnSignal::setBlinkRight(bool state){
    _blink_right=state;
    if(!state)digitalWrite(TURNSIGNAL_R,LOW);
}

volatile bool TurnSignal::_blink_right = false;
volatile bool TurnSignal::_blink_left = false;

//class Buzzer
Buzzer::Buzzer(){
    pinMode(BUZZER_PIN,OUTPUT);
}

void Buzzer::setFrequency(unsigned int frequency){
    _frequency=frequency;
}

void Buzzer::start(){
    tone(BUZZER_PIN,_frequency);
}

void Buzzer::stop(){
    noTone(BUZZER_PIN);
}