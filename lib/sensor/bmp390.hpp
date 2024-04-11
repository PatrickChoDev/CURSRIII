#include <sensor.hpp>

// TODO Implement address configuration
#ifndef BMP390_ADDRESS
#define BMP390_ADDRESS 0x77
#endif

#ifndef BMP390_TEMP_OVERSAMPLING_MODE
#define BMP390_TEMP_OVERSAMPLING_MODE BMP3_OVERSAMPLING_8X
#endif

#ifndef BMP390_PRESSURE_OVERSAMPLING_MODE
#define BMP390_PRESSURE_OVERSAMPLING_MODE BMP3_OVERSAMPLING_4X
#endif

#ifndef BMP390_IIR_FILTER_COEFF
#define BMP390_IIR_FILTER_COEFF BMP3_IIR_FILTER_COEFF_3
#endif

#ifndef BMP390_ODR
#define BMP390_ODR BMP3_ODR_50_HZ
#endif

#ifndef BMP390_ENABLED
#define BMP390_ENABLED 1
#endif

#ifndef BMP390_SEE_LEVEL_PRESSURE
#define BMP390_SEE_LEVEL_PRESSURE 1013.25
#endif