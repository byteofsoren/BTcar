/* This is the configuration file for the project */
/* Each subsection contains one part of the configuration */


/* Pin set up for steering and motor control */

#define MOTOR_PIN 5
#define STEERING_PIN 6


/* Pin set up for the BT module */

#define SOFTWARE
#define RX_PIN 7
#define TX_PIN 4


// flags [ACC][enable system][enable steering][enable motor] if flags==0 then em stop.
/* Flags macros to cut out flag bits */
#define MOTOR_ENABLE(f) f & 0b1
#define STEERING_ENABLE(f) (f >> 1)& 0b1
#define SYS_ENABLE(f) (f >> 2)& 0b1
#define SET_ACC(f) f |= (1 << 3)

/* Macro for testing the servos */
/* servo: is the servo object */
#define SWEEP(servo) for (int i = 0; i < 180; i++) {\
    servo.write(i);\
    delay(10);} for (int i = 180; i > 0; i--) {\
        servo.write(i);\
        delay(10);}

