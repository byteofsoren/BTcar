/* Controller for the Bluetooth car is written by
 * Magnus Sörensen, byteofsoren@gmail.com
 *
 * LICENCE: Fuck, or do what ever you f..n want
 */

#include "Arduino.h"
#include "config.h"
#include <Servo.h>

/* typedef for function pointers */
typedef int (*fptr)( int );

/* Macros for decoding the flags in carSerial */
#define FLAGS_ISENABLED(f) (f & 0b1)
#define FLAGS_SETENABLED(f,state) f |= (state & 0b1)
#define FLAGS_SETACC(f) f |= 1 << 2
#define FLAGS_ISDISCONNECT(f) (f & (1<<2))
#define FLAGS_SETSTATE(f,state) f |= ((state & 0b1111) << 3)


/* carSerial is the communication to be expected in both directions */
struct carSerial {
    char motor_speed;
    char steering_angle;
    char flags; /* [4 bit System state][Dis connect request][ACC][system enable]*/
}carSerial = {0,0,0};

/* carResources is the system resource container */
struct carResourses {
    int state;
    int serialState;
    unsigned int serialReadLastTime;
    Servo motor;
    Servo steering;
}carResourses = {0,0,0};


/* This function reads the serial buss and stores the results in the carSerial struct */
/* The function is non blocking and will return how many chars was read or zero if none was read*/
int read_serial()
{
    if (Serial.available() >= 1) {
        /* Data alible on the serial buss */
        switch (carResourses.serialState) {
            case 0:
                carSerial.motor_speed = Serial.read();
                carResourses.serialState++;
                break;
            case 1:
                carSerial.steering_angle = Serial.read();
                carResourses.serialState++;
                break;
            case 3:
                carSerial.flags = Serial.read();
                carResourses.serialState = 0; // Reset the counter
                break;
        }
        carResourses.serialReadLastTime = millis();
        return 1;
    }
    return 0;
}

/* This function sends replies back to the master */
/* The arguments are the state of the system and the if it is a acc you want to send */
void write_serial(int acc)
{
    if(acc != 0)
        FLAGS_SETACC(carSerial.flags);
    FLAGS_SETSTATE(carSerial.flags,carResourses.state);
    Serial.write(carSerial.flags);
}

int car_init(int a)
{
    delay(500);
    write_serial(0);
    /* This part is blocking to wait for connection */
    while(FLAGS_ISENABLED(carSerial.flags) != 1)
    {
        read_serial();
        delay(10);
    }
    carResourses.state = CAR_ESTABLISH;
    return 0;
}

int car_establish(int a)
{
    /* Also blocking until the steering angle and motor speed is set to STEERING_CENTER and MOTOR_OFF */
    /* This is do to safety */
    while((carSerial.steering_angle == STEERING_CENTER) && (carSerial.motor_speed == MOTOR_OFF))
    {
        read_serial();
        delay(10);
    }
    /* Change state of the system to running */
    if (FLAGS_ISDISCONNECT(carSerial.flags) == 1 ) {
        carResourses.state = CAR_DISCONNECT_REQUEST;
    }else{
        if (! carResourses.motor.attached())
            carResourses.motor.attach(MOTOR_PIN);
        if (! carResourses.steering.attached())
            carResourses.motor.attach(STEERING_PIN);
        FLAGS_ISENABLED(carSerial.flags);
        carResourses.state = CAR_RUNNING;
    }
    /* Send an acc to signal that the car is in CAR_RUNNING mode */
    write_serial(1);
    return 0;
}

int car_running(int a)
{
    read_serial();
    if ((FLAGS_ISDISCONNECT(carSerial.flags) != 0) && (carResourses.serialReadLastTime > COMUNICATION_DELAY)) {
        /* Dis connect if ether disconnect request or the connection seams to be lost */
        carResourses.state = CAR_DISCONNECT_REQUEST;
    } else if (FLAGS_ISENABLED(carSerial.flags) == 1)
    {
        carResourses.motor.write(carSerial.motor_speed);
        carResourses.steering.write(carSerial.steering_angle);
        write_serial(1);
    } else
    {
        carResourses.motor.write(MOTOR_OFF);
        carResourses.steering.write(STEERING_CENTER);
        write_serial(1);
    }
    return 0;
}

int car_disconnect(int a)
{
    /* A disconnect request have bean sent to the Arduino */
    FLAGS_SETSTATE(carSerial.flags, 1);
    carResourses.state = CAR_INIT;
    write_serial(1); // Send an acc
    /* Stop the car */
    carResourses.motor.write(MOTOR_OFF);
    carResourses.steering.write(STEERING_CENTER);
    delay(1000);
    /* Detach the servo */
    carResourses.motor.detach();
    carResourses.steering.detach();
    return 0;
}

void setup()
{
    Serial.begin(9600);
    fptr stateActions[CAR_MAXSTATES];
    int counter = 0;
    /* Assign functions to each state */
    stateActions[CAR_INIT] = car_init;
    stateActions[CAR_ESTABLISH] = car_establish;
    stateActions[CAR_RUNNING] = car_running;
    stateActions[CAR_DISCONNECT_REQUEST] = car_disconnect;
    /* RUN the state machine */
    while(1)
    {
        stateActions[carResourses.state](counter);
        counter++;
    }
}


/* Depiction of loop function only exist for compiler legacy */
void loop()
{
    //SWEEP(car.motor);
    //SWEEP(steering);
}
