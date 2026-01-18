#pragma once
#include "GpsData.h" // GpsData struct definition

#define NMEA_MAX_LEN 85 // Standart NMEA sentence maximum length is 82 chars

class NmeaParser {
public:
    NmeaParser();

    // Every byte received from GPS module is fed to this function
    // If a complete sentence is formed, it is parsed and returned true
    bool processByte(char c, GpsData& outData) ;

private:
    char _buffer[NMEA_MAX_LEN];
    uint8_t _bufferIndex;

    // Main function to parse a complete NMEA sentence
    bool parseSentence(char* sentence, GpsData& outData);

    // NMEA Checksum validation
    // $GPRMC,.....*HH\r\n -> $ and * characters include XOR calculation
    bool validateChecksum(const char* sentence);

    // RMC distinguish sentence (Recommended Minimum Navigation Information)
    bool parseGPRMC(char* sentence, GpsData& outData);

    // NMEA format (DDMM.MMMM) converts to Decimal Degrees (DD.DDDDDD) 
    float convertToDecimal(const char* val);
};