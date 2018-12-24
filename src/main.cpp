/* Controller for the Bluetooth car is written by
 * Magnus Sörensen, byteofsoren@gmail.com
 *
 * LICENCE: Fuck, or do what ever you f..n want
 */

#include "Arduino.h"
#include "config.h"
#include <Servo.h>

/* Create the motor and steering servos */
/* Servo motor; */
/* Servo steering; */

struct car {
    char motor_speed;
    char steering_angle;
    unsigned char flags; // flags [ACC][enable system][enable stering][enable motor] if flags==0 then em stop.
    Servo motor;
    Servo steering;
}car = {0,0,0};

void setup()
{
    Serial.begin(9600);
    delay(500);
    Serial.println(200);
    int cont = 0;
    while (cont != 232)
    {
        while(Serial.available() < 1);
        cont = Serial.read();
    }
    /* System ready */
    delay(10);
    car.motor.attach(MOTOR_PIN);
    car.steering.attach(STEERING_PIN);
    /* motor.attach(MOTOR_PIN); */
    /* steering.attach( STEERING_PIN); */
}

void loop()
{
    /* servo value storage */
    /* Is there any orders in the buffer? */
    if(Serial.available() > 2)
    {
        /* Read the serial to get the values. */
        car.motor_speed = Serial.read();
        car.steering_angle = Serial.read();
        delay(10);
        /* Return the flags to the sender as an ACC */
        car.flags = Serial.read();
    }

    if (MOTOR_ENABLE(car.flags) && STEERING_ENABLE(car.flags) && SYS_ENABLE(car.flags)) {
        car.motor.write(car.motor_speed);
        car.steering.write(car.motor_speed);
    }
    delay(10);
    if (car.flags != 0)
    {
        SET_ACC(car.flags);
        Serial.write(car.flags);
    }
    //SWEEP(motor);
    //SWEEP(steering);
}
