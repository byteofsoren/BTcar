# Blue Tooth car
In this project the basis for steering a RC car over Blue Tooth(BT) is explored.
Unfortunately I burnt my BT module and thus haven't had the chance to test it.

## Installation and compiling the project
The program is written using PlatformIO as base so perhaps the user needs to install that program first.
On ArchLinux it is:

    $ pikaur -S platformio-git

Debian/Ubuntu:

    $ sudo apt-get install python-pip
    $ sudo pip install -U platformio

Clone the repo from GitHub.

    $ git clone https://github.com/byteofsoren/BTcar.git
    $ cd BTcar

When the project was downloaded the PlatformIO package tries to auto connect the Arduino.
In some cases the PlatformIO could not find the system. The fix to this problem is to tell PlatformIO to to use a specific port. This could be done by editing the `platformio.ini` and change and uncomment the line:

    ;upload_port = /dev/ttyUSB3

After that the system could be compiled and uploaded with the following command:

    $ pio run -v
    $ pio run -v -t upload

# Code Configurations
This project have an configuration file in `src/config.h` that tells the system where the motors and other pretrials is connected to the system. It also have important parameters like where the motor is standing still and the steering is centred.

# Code Documentation
In this part an explanation on how the code works follows:
The system is constructed with an connection state machine in mind.
This state machine makes sure that the car don't go out of control if the system looses the connection to the master.

    State_init:
        Send flags to master.
        Wait for resiveng anable signal form master.
        If not connected connect the motor cortroller and the servo.
        Set state to CAR_ESTABLISH:

    State_establish:
        Wait for resiveng stering center and motor off form master:
        Did the system recive a disconnect request?
            If so then set state to CAR_DISCONNEC_REQUEST
        Else set motor speed and steraning to the values given from the master.
        And set the system to CAR_RUNNING.

    state_running:
        If the system recive the disconnect request or a communication time out:
            set state of the car to CAR_DISCONNECT_REQUEST.
            And then exit.
        if the car recive the car enable flags the the motor and steernig is set.
        else the motor is set to stop and the steraing to ceter.
        return an ACC to the sender.

    state_disconnect:
        Set states init:
        send an acc.
        stop the motor and center the steernig.
        disconnect the motor controller and the steernig servo.

