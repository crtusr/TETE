#ifndef NTXTOOL_H
#define NTXTOOL_H
  #include<stdint.h>
  #include<stdio.h>


  typedef struct IHeader
  {
    uint16_t signature;   // +2 = 2
    uint16_t compVersion; // +2 = 4
    uint32_t rootOff;       // +4 = 8
    uint32_t nextUnusedOff; // +4 = 12
    uint16_t itemSize;    // +2 = 14
    uint16_t keySize;     // +2 = 16
    uint16_t keyDecimals; // +2 = 18
    uint16_t maxItems;    // +2 = 20
    uint16_t minItems;    // +2 = 22
    char keyExpr[257]; // +257 = 279
    uint8_t uniqueFlag;   // +1 = 280
  }IHeader;

  typedef struct
  {
    uint32_t leftPagePointer;
    uint32_t dbfRecNo;
    char key[256];
  }IndexEntry;

  static const size_t PAGESIZE = 1024;

  void storeIndexHeader(IHeader* iHead, FILE* fPtr);
  size_t getIndexedIndex(char* key, IHeader* iHead, FILE* fPtr);
  int makeIndex(char* key, FILE* dbfFptr, char* iName);
  int makeIndexShort(char* key, char* fName, char* iName);
#endif
