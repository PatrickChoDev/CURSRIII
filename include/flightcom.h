// #define PRELAUNCH_DELAY 100
// #define BURNOUT_DELAY 100
// #define COASTING_DELAY 100
// #define APOGEE_ACCELERATION_THRESHOLD 8
// #define APOGEE_PRESSURE_THRESHOLD 1000

// #define POSTFLIGHT_ACCELERATION_THRESHOLD 8.8

// #define POSTFLIGHT_DELAY 100

// #define DROUGE_PARACHUTE_EJECTION_PIN 22
// #define DROUGE_PARACHUTE_HIGH_DELAY 1000

// #define MAIN_PARACHUTE_EJECTION_PIN 21
// #define MAIN_PARACHUTE_HIGH_DELAY 1000

// RUN_ID is a unique identifier for the flight. It is used to name the log files
#define RUN_ID 1
#define RESET_FLIGHT false

#define PRELAUNCH_DELAY 100000
#define BURNOUT_DELAY 2500
#define COASTING_DELAY 10000
// #define APOGEE_ACCELERATION_THRESHOLD 0.2
#define APOGEE_PRESSURE_THRESHOLD 27300

// #define POSTFLIGHT_ACCELERATION_THRESHOLD 8.8

#define POSTFLIGHT_DELAY 100

#define DROUGE_PARACHUTE_EJECTION_PIN 22
#define DROUGE_PARACHUTE_HIGH_DELAY 1000

#define MAIN_PARACHUTE_DELAY 15000

#define MAIN_PARACHUTE_EJECTION_PIN 21
#define MAIN_PARACHUTE_HIGH_DELAY 1000

#define TESTING_FLIGHTSTAGE false

#include <memory.hpp>
#include <data.hpp>
#include <radio.hpp>
#include <sensor.hpp>