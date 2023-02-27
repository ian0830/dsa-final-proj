#define encodingSpace 36
#define upperBound 2147483647

#include <stdio.h>

int charIndex(char c) {
    if (c >= 48 && c <= 57) {
        return (int)c - 48;
    }
    if (c >= 65 && c <= 90) {
        return 10 + (int)c - 65;
    }
    if (c >= 97 && c <= 122) {
        return 10 + (int)c - 97;
    }
}

unsigned long long calculateChecksum(char* string, int length) {
    unsigned long long checksum = 0;
    for (int index = 0; index < length; index++) {
        checksum = (checksum * encodingSpace + charIndex(string[index])) % upperBound;
    }
    return checksum;
    // remember not to overflow
}

int main() {
    char * a = "In";
    char * b = "Normal";
    printf("%lld\n", calculateChecksum(a, 2));
    printf("%lld\n", calculateChecksum(b, 6));
}