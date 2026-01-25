#include "NmeaParser.hpp"
#include <stdio.h>
#include <cstring>
#include <cstdlib>
#include <cmath>

NmeaParser::NmeaParser() : _bufferIndex(0) {
    memset(_buffer, 0, NMEA_MAX_LEN);
}

bool NmeaParser::processByte(char c, GpsData& outData) {
    // 1. Start character control ($)
    if (c == '$') {
        _bufferIndex = 0; // Buffer reset
    }

    // Buffer overflow control
    if (_bufferIndex >= NMEA_MAX_LEN - 1) {
        _bufferIndex = 0; // Error: buffer overflow, reset
        return false;
    }

    // Store character in buffer
    _buffer[_bufferIndex++] = c;

    // 2. End character control (\n)
    if (c == '\n') {
        _buffer[_bufferIndex] = '\0'; // String terminator
        return parseSentence(_buffer, outData);
    }

    return false;
}

bool NmeaParser::parseSentence(char* sentence, GpsData& outData) {
    // 1. Checksum control
    if (!validateChecksum(sentence)) return false;

    // 2. Sentence type detection
    // $GPRMC or $GNRMC (GPS+GLONASS) etc.
    if (strstr(sentence, "RMC")) { 
        return parseGPRMC(sentence, outData);
    }

    return false;
}

bool NmeaParser::validateChecksum(const char* sentence) {
    const char* p = sentence + 1; // skip $
    uint8_t calculated = 0;

    // XOR processing until '*' or end of string
    while (*p != '*' && *p != '\0') {
        calculated ^= *p;
        p++;
    }

    if (*p != '*') return false; // * not found

    // Read checksum (Hex)
    int received;
    sscanf(p + 1, "%2x", &received);

    return (calculated == received);
}


bool NmeaParser::parseGPRMC(char* sentence, GpsData& outData) {
    // Exp: $GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
    // strtok is not thread-safe but we are in a single-threaded context here.
    // Still manual pointer advancement is safer but for simplicity we'll use a custom tokenizer.
    
    char* token;
    int fieldIndex = 0;
    
    // $GPRMC
    token = strtok(sentence, ","); 
    
    while ((token = strtok(NULL, ",")) != NULL) {
        fieldIndex++;
        switch (fieldIndex) {
            case 2: // Status (A=Active, V=Void)
                outData.isValid = (token[0] == 'A');
                break;
            case 3: // Latitude (4807.038 -> 48 deg 07.038 min)
                if (outData.isValid) outData.latitude = convertToDecimal(token);
                break;
            case 4: // N/S
                    if (outData.isValid && token[0] == 'S') outData.latitude *= -1;
                break;
            case 5: // Longitude (01131.000 -> 11 deg 31.000 min)
                if (outData.isValid) outData.longitude = convertToDecimal(token);
                break;
            case 6: // E/W
                    if (outData.isValid && token[0] == 'W') outData.longitude *= -1;
                break;
            case 7: // Speed in Knots
                if (outData.isValid) {
                    float knots = strtof(token, NULL);
                    outData.speedKmh = knots * 1.852f; // Knot -> Km/h
                }
                break;
        }
    }
    return outData.isValid;
}

float NmeaParser::convertToDecimal(const char* val) {
    float raw = strtof(val, NULL); // Exp: 4102.50 (41 degree, 2.5 min)
    int dd = (int)(raw / 100);     // 41
    float mm = raw - (dd * 100);   // 2.50
    return dd + (mm / 60.0f);      // 41 + 0.0416...
}