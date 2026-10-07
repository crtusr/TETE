#ifndef UTILITIES_H
#define UTILITIES_H
  
  #include<stdint.h>
  #include<limits.h>
  #include<string.h>
  #include<stdio.h>
  #include<stdlib.h>

  #define MAX_FIELD_LENGTH 254
uint16_t readU16LE(uint8_t* buffer);
uint32_t readU32LE(uint8_t* buffer);
void writeU16LE(uint8_t* buffer, uint16_t val);
void writeU32LE(uint8_t* buffer, uint32_t val);
unsigned long long int iPow(unsigned long long int base, int exp);
void bSortStr(char* buffer, int* index, size_t size, size_t* off);
void bSortStr2(char* buffer, uint32_t* index, size_t size, size_t len);
void zeroFill(char* sto, size_t size);
void spaceFill(char *string, size_t size);
void rightAlignV2(char *string, const size_t size);
void delLeadingZeroes(char *string, size_t size);
void rightAlign(char *string, const size_t size);
#endif
