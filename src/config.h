/* This is the configuration file for the project */
/* Each subsection contains one part of the configuration */


/* Pin set up for steering and motor control */

#define MOTOR_PIN 5
#define STEERING_PIN 6


/* Pin set up for the BT module */

#define SOFTWARE
#define RX_PIN 7
#define TX_PIN 4

/* States of the car */
#define CAR_INIT 0
#define CAR_ESTABLISH 1
#define CAR_RUNNING 2
#define CAR_DISCONNECT_REQUEST 4
#define CAR_MAXSTATES 5

/* Servo settings */
#define MOTOR_OFF 127
#define STEERING_CENTER 127
#define COMUNICATION_DELAY 600 // milliseconds



/* Macro for testing the servos */
/* servo: is the servo object */
#define SWEEP(servo) for (int i = 0; i < 180; i++) {\
    servo.write(i);\
    delay(10);} for (int i = 180; i > 0; i--) {\
        servo.write(i);\
        delay(10);}

