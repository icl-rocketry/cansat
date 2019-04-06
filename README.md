ICLR CanSat Documentation
=========================

This is the repository containing the code for the Imperial College London Rocketry CanSat, developped by the payload team.

## Installation instructions

To get started with the code, clone the repository to your local machine, then compile and upload the code using the file `MasterCan.ino`.

To compile the code succesfully, the following non-standard libraries must be installed:

* [DFRobot BMP388](https://github.com/DFRobot/DFRobot_BMP388)

* [Adafruit BNO055](https://github.com/adafruit/Adafruit_BNO055)

* [Adafruit Unified Sensor](https://github.com/adafruit/Adafruit_Sensor)

The latter two can be installed through the library manager using the Arduino IDE.

## Instructions for Operation of the CanSat

Upon connecting the power, the CanSat should turn on automatically and begin setup. The **green** LED will turn on to indicate this, and will remain on as long as it is powered.

### Errors

In the event that the CanSat encounters an error throughout its operation, it will save the code of the error to a variable, named `softState`.  
It is a single byte where the error codes are each single bits. The individual errors can be found by getting the softState integer and converting it to binary. Alternatively, one can decompose it to its component powers of 2, as follows:

```
1	-	WARNING: Results file not detected, created
2	-	ERROR: SD card initialisation failed
4	-	ERROR: BNO055 initialisation failed
8	-	ERROR: Failed to write to SD Card
16	-	ERROR: Battery voltage low
```

For example, a softState of 49 would mean:

* Battery Voltage is low

* It is failing to write to SD card

* The results file wasnt originally present on the SD Card, so it has been recreated.

Following from this, the softState can never be greater than 63.

#### Critical and non-Critical Errors

Some errors mean that the CanSat will be unable to complete its mission. Such errors occur when a sensor or the SD card has failed to initialise. These will be referred to as "critical errors".

In the event of a **critical error**, the CanSat will return the softState via serial, the **red** LED will **blink**, and the buzzer will buzz intermitently until the Arduino is reset. During this state, no normal operations such as data collection are carried out.

In the event of a **non-critical error**, the **red** LED will **turn on** without blinking. The buzzer will remain silent. Otherwise the CanSat will continue with normal operation.

### Landing Detection

When the CanSat detects that it has landed, the buzzer will turn on and emit a continuous audio beacon. It will continue until the Arduino is reset.

### Data structure type

To save storage space and make the CanSat more reliable, the data packet is as minimalistic as possible. It has the following structure:

`packetCount:flightTime:pressure:temperature:altitude:velocity:batteryVoltage:softState:Xacceleration:Yacceleration:Zacceleration`

All units are in SI.
