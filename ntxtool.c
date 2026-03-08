#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#include"utilities.h"
#include"ntxtool.h"
#include"dbftool.h"

static size_t findField(char* fieldName, descriptor* descr, size_t nOfDescr)
{
  int check;
  for(size_t i = 0; i < nOfDescr; i++)
  {
    check = !strncmp(fieldName, descr[i].fieldname, strnlen(fieldName, 11));
    if(check) return i;
  }
  return NOT_FOUND;
}

static uint16_t calculateMaxPageEntries(uint16_t keySize)
{
  const size_t PADDING = 32;
  const size_t entrySize = keySize + sizeof(uint32_t) * 2 + sizeof(uint16_t);
  
  //this should yield 990
  const size_t availableSpace = PAGESIZE - PADDING - sizeof(uint16_t);
  
  uint16_t max = (((float)availableSpace)/((float)entrySize) + 0.5) / 2;
  return max * 2;
}
void storeIndexHeader(IHeader* iHead, FILE* fPtr)
{
  fread(&iHead->signature, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->compVersion, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->rootOff, sizeof(uint32_t), 1, fPtr);
  fread(&iHead->nextUnusedOff, sizeof(uint32_t), 1, fPtr);
  fread(&iHead->itemSize, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->keySize, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->keyDecimals, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->maxItems, sizeof(uint16_t), 1, fPtr);
  fread(&iHead->minItems, sizeof(uint16_t), 1, fPtr);
  fread(iHead->keyExpr, sizeof(char), sizeof(iHead->keyExpr)/sizeof(char), fPtr);
  fread(&iHead->uniqueFlag, sizeof(uint8_t), 1, fPtr);
}

size_t getIndexedIndex(char* key, IHeader* iHead, FILE* fPtr)
{
  /*
   *  This gets the index from the .NTX file (opened previously) this searches
   *  linearly per page, binary search would be faster but 1) it should find
   *  the first appearance of a key 2) it would make the algorithm more complex 
   * */
  uint32_t currPage = iHead->rootOff;
  uint16_t nOfPageInd = 0;
  uint16_t pagePtrs[120] = {0};
  uint32_t  ind;
  uint32_t  leftPage;
  uint32_t  dbfIndex;
  char buffer[255];
  int check;
  while(1)
  {
    check = -1;
    ind = 0;
  	fseek(fPtr, currPage, SEEK_SET);
    fread(&nOfPageInd, sizeof(uint16_t), 1, fPtr);
    fread(pagePtrs, sizeof(uint16_t), nOfPageInd + 1, fPtr); // +1 because of the "right page pointer"
    while(ind <= nOfPageInd && check < 0)
    {
      memset(buffer, 0, 255);
      fseek(fPtr, currPage, SEEK_SET);
      fseek(fPtr, pagePtrs[ind], SEEK_CUR);
      fread(&leftPage, sizeof(uint32_t), 1, fPtr);
      fread(&dbfIndex, sizeof(uint32_t), 1, fPtr);
      fread(buffer, sizeof(char), iHead->keySize, fPtr);
      //printf("%d %d\n%d %d %s", currPage, pagePtrs[ind], leftPage, dbfIndex, buffer);
      check = strncmp(buffer, key, iHead->keySize);
      if(check == 0) return dbfIndex; // found
      else if((check > 0 && leftPage) || (leftPage && ind == nOfPageInd)) currPage = leftPage; //key is in child node
      else if((check > 0 && !leftPage) || ind >= nOfPageInd) return -1; //Not found
      ind++;
    }
  }
}

int makeIndex(char* fieldName, FILE* fPtr, char* iName)
{ 
  /*
   * these are values that I don´t know exactly how to interpret them. I will
   * leave them as the values of the indexes im working on so the compatibility
   * with our use case remains the same
   */
  #define SIGNATURE 6
  #define COMP_VERSION 1
  #define UNIQUE_FLAG 0
  int err = 0;

  // variables
  header head[1];
  descriptor descr[256];
  IHeader iHead[1];
  IndexEntry entry[1];
  FILE *indexPtr = NULL;
  uint32_t *index = NULL;
  char *buffer = NULL;
  unsigned char *pageBuffer = NULL;


  size_t nOfDescr = store_header_data(head, fPtr, 0);
  store_descriptor_data(descr, fPtr);
  
  int descrIndex = findField(fieldName, descr, nOfDescr);
  
  if(descrIndex == NOT_FOUND) return descrIndex;

  const size_t maxPageEntries = calculateMaxPageEntries(descr[descrIndex].length);

  // entry size would be itemSize + offset
  const size_t entrySize = descr[descrIndex].length + sizeof(uint32_t) * 2 + sizeof(uint16_t);
  const size_t minPageEntries = maxPageEntries / 2;
  const unsigned int nOfLevels = log(head->nofrecords) / log(maxPageEntries);
  
  iHead->signature = SIGNATURE;
  iHead->compVersion = COMP_VERSION;
  iHead->rootOff = 0;
  iHead->nextUnusedOff = 0;
  iHead->itemSize = entrySize - 2;
  iHead->keySize = descr[descrIndex].length;
  iHead->keyDecimals = descr[descrIndex].decimal;
  iHead->maxItems = maxPageEntries;
  iHead->minItems = minPageEntries;
  memset(iHead->keyExpr, 0,sizeof(iHead->keyExpr));
  memcpy(iHead->keyExpr, fieldName, strnlen(fieldName, 255));
  iHead->uniqueFlag = UNIQUE_FLAG;

  /*
   * this should set the stride for every level, but it should skip 1 entry per
   * maxPageEntries
   *
   * levelOff[0] = 1 
   * levelOff[1] = maxPageEntries + 1 
   * levelOff[2] = maxPageEntries * maxPageEntries + 1
   * etc.
   * */

  uint32_t levelOff[255] = {0}; 

  // Dynamic buffers
  index = malloc((head->nofrecords + 20) * sizeof(int));
  if(index == NULL)
  {
    err = MEM_ISSUES;
    goto error;
  }
  memset(index, -1, (head->nofrecords + 20) * sizeof(int));
  buffer = malloc(head->nofrecords * descr[descrIndex].length);
  if(buffer == NULL)
  {
    err = MEM_ISSUES;
    goto error;
  }
  pageBuffer = malloc(PAGESIZE);
  if(pageBuffer == NULL)
  {
    err = MEM_ISSUES;
    goto error;
  }
  memset(pageBuffer, 0, PAGESIZE);
  uint16_t entryOffset[100] = {0};

  //preliminary calculations
  
  for(unsigned int level = 0; level <= nOfLevels; level++)  levelOff[level] = iPow(maxPageEntries + 1 , level);

  // extract all dbf file contents of that field
  
  for(uint32_t i  = 0; i < head->nofrecords; i++)
  {
    index[i] = i;
    get_data(&buffer[i * iHead->keySize], i, fieldName, fPtr, head, descr);
  }
  
  bSortStr(buffer, index, head->nofrecords, iHead->keySize);
  
  //set page offsets since it should be always the same in a clean page
  for(size_t i = 0; i <= maxPageEntries; i++)
  {
    //this is for checking that everything is written in its due offset
    entryOffset[i] = (maxPageEntries + 2) * sizeof(uint16_t) + i * iHead->itemSize;
  }

  // Writing entries
  writeU16LE(&pageBuffer[0], maxPageEntries);
  memcpy(&pageBuffer[1 * sizeof(uint16_t)], entryOffset, (maxPageEntries+ 1) * sizeof(uint16_t));

  indexPtr = fopen(iName, "wb");
  if(indexPtr == NULL)
  {
    err = CANNOT_OPEN_FILE;
    goto error;
  }
  fseek(indexPtr, PAGESIZE, SEEK_CUR);
  
  //this populates the page with entries for every level, there is something i'm missing
  //nOfEntries is off on the last level per page
  uint32_t currLevel = 0;
  uint16_t nOfEntries;
  entry->leftPagePointer = 0;
  while(currLevel <= nOfLevels)
  {
    int64_t recCount = -1;
    while(recCount < head->nofrecords)
    {
      uint16_t check = (head->nofrecords - recCount)/levelOff[currLevel];
      nOfEntries = check > maxPageEntries ? maxPageEntries : check;
      if(nOfEntries)
      {
        //printf("\n\n%d %lld %d %d\n\n", head->nofrecords, recCount, nOfEntries, levelOff[currLevel]);
        writeU16LE(&pageBuffer[0], nOfEntries);
      //write the page
        recCount += levelOff[currLevel]; 
        if(recCount < head->nofrecords)
        {
          for(int i = 0; i < nOfEntries && recCount < head->nofrecords; i++)
          {
            entry->leftPagePointer += PAGESIZE * (currLevel > 0 ? 1 : 0); // it could be faster
            writeU32LE(&pageBuffer[entryOffset[i]], entry->leftPagePointer);
            writeU32LE(&pageBuffer[entryOffset[i] + 4], index[recCount] + 1);
            memcpy(&pageBuffer[entryOffset[i] + 8], &buffer[index[recCount] * iHead->keySize], iHead->keySize);
            recCount += levelOff[currLevel]; 
          }
        entry->leftPagePointer += PAGESIZE * (currLevel > 0 ? 1 : 0); // it could be faster
        writeU32LE(&pageBuffer[entryOffset[nOfEntries]], entry->leftPagePointer);

        //push the page to the file
        fwrite(pageBuffer, 1, PAGESIZE, indexPtr);
        //printMem(pageBuffer, PAGESIZE, 1);
        //printPage(pageBuffer);
        }
      }
      else
      {
        recCount += levelOff[currLevel];
      }
    }
    currLevel++;
  }

  iHead->rootOff = entry->leftPagePointer + PAGESIZE;
  fseek(indexPtr, 0, SEEK_SET);
  fwrite(iHead, 1, sizeof(iHead), indexPtr);
  
  if(fclose(indexPtr) != 0) goto error;
  free(pageBuffer);
  free(buffer);
  free(index);
  return 0;

error:

  if(indexPtr != NULL) fclose (indexPtr);
  if(pageBuffer) free(pageBuffer);
  if(buffer) free(buffer);
  if(index) free(index);
  return err;
}

int makeIndexShort(char* key, char* fName, char* iName)
{
  FILE* dbfPtr = NULL;
  dbfPtr = fopen(fName, "rb");
  if(dbfPtr == NULL)
  {
    return CANNOT_OPEN_FILE;
  }

  int err = makeIndex(key, dbfPtr, iName);

  if(!err) printf("index %s was succesfully created", iName);
  else
  {
    return err;
  }

  if(fclose(dbfPtr) != 0)
  {
    return CANNOT_CLOSE_FILE;
  }
  return 0;
}

