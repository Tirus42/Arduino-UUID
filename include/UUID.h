#pragma once

#include <ESPRandom.h>
#include <Arduino.h>

/**
* UUID-v4 struct. Stores the UUID as bytes representation.
* Provides functions to parse from string, generate a string
* and generate a new UUID.
*/
struct UUID {
    std::array<uint8_t, 16> bytes;

    /**
    * Default constructor. On default generates a new UUID.
    */
    UUID(bool generateNew = true) :
        bytes() {

        if (generateNew) {
            generate();
        }
    }

    /**
    * Construct a UUID by the given UUID-string.
    * Note that there is no error checking. If the supplied
    * string is not in the expected format, the behavior and result is undefined.
    */
    UUID(const char* uuidString) :
        bytes() {

        parseFrom(uuidString);
    }

    uint8_t _hexToHalfByte(const char hexChar) {
        if (hexChar >= '0' && hexChar <= '9') {
            return hexChar - '0';
        }

        return hexChar - 'a' + 10;
    }

    uint8_t _parseTwoHexDigits(const char* ptr) {
        uint8_t v0 = _hexToHalfByte(ptr[0]);
        uint8_t v1 = _hexToHalfByte(ptr[1]);

        return v0 << 4 | v1;
    }

    /**
    * Parses the given string and extracts a UUID.
    * The expected format is: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    * (hex string in lowercase).
    * Note: Does not perform any error checking.
    * Results in undefined behavior when the given string is not of the expected format.
    */
    void parseFrom(const char* uuidString) {
        size_t offset = 0;
        size_t i = 0;

        while (i < 36) {
            if (i == 8 || i == 13 || i == 18 || i == 23) {
                i++;
                continue;
            }

            bytes[offset++] = _parseTwoHexDigits(uuidString + i);
            i += 2;
        }
    }

    /**
    * Generates a new random UUID using the ESPRandom library.
    */
    void generate() {
        ESPRandom::uuid4(bytes.data());
    }

    /**
    * \returns the UUID in the string representation: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    */
    String toString() const {
        return ESPRandom::uuidToString(bytes.data());
    }

    bool operator<(const UUID& other) const {
        return bytes < other.bytes;
    }

    bool operator==(const UUID& other) const {
        return bytes == other.bytes;
    }
};
