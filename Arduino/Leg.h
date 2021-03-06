#ifndef LEG_H
#define LEG_H

#include <Servo.h>
#include <Arduino.h>
#include <stdint.h>


class Leg{

	private:

		uint8_t leg_id;
		uint8_t shoulder_angle;
		uint8_t elbow_angle;
		uint8_t foot_angle;

	public:

		Leg(uint8_t id); //Constructor of the Leg class

		~Leg(); //Destructor of the Leg class

		uint8_t getID();

		uint8_t getFoot();

		uint8_t getElbow();

		uint8_t getShoulder();

		void simulate_actuate();

		void setFoot(uint8_t angle);

		void setElbow(uint8_t angle);

		void setShoulder(uint8_t angle);

		void actuate(Servo *shoulder_servo, Servo *elbow_servo, Servo *foot_servo);

		void addOffset(uint8_t shoulder_off, uint8_t elbow_off, uint8_t foot_off);
};

#endif
