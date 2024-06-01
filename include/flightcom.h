#define BURNOUT_DELAY 5000
#define COASTING_DELAY 10000
#define APOGEE_ACCELERATION_THRESHOLD 8
#define POSTFLIGHT_ACCELERATION_THRESHOLD 8.8

// RUN_ID is a unique identifier for the flight. It is used to name the log files
#define RUN_ID "1"

#include <memory.hpp>
#include <data.hpp>
#include <radio.hpp>
#include <sensor.hpp>