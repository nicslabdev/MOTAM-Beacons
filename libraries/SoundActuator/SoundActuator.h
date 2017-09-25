/*
  SoundActuator.h - Interface to be implemented by actuators 
  that want to emit sound signals to user. 
  Created by Jesus Rodriguez, March 24, 2015.
  Developed for DEPHISIT project. 
*/
#ifndef SoundActuator_h
#define SoundActuator_h

class SoundActuator
{
  public:
	virtual void playSound(){};
};

class SoundActuatorBasedOnGroveBuzzer : public SoundActuator
{  
  public:
	SoundActuatorBasedOnGroveBuzzer(int buzzerDigitalPin);
	void begin();
	void playSound();
  private:
    int _buzzerPin;
	void playTone(int tone, int duration);
};  

#endif