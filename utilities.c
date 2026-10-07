#include "utilities.h"

uint16_t readU16LE(uint8_t* buffer)
{
  return (buffer[1] << 8) | (buffer[0]);
}

uint32_t readU32LE(uint8_t* buffer)
{
  return (buffer[3] << 24) | (buffer[2] << 16) | (buffer[1] << 8) | (buffer[0]);
}

void writeU16LE(uint8_t* buffer, uint16_t val)
{
  buffer[0] = val & 0xff;
  buffer[1] = (val >> 8) & 0xff;
  return;
}

void writeU32LE(uint8_t* buffer, uint32_t val)
{
  buffer[0] = val & 0xff;
  buffer[1] = (val >> 8) & 0xff;
  buffer[2] = (val >> 16) & 0xff;
  buffer[3] = (val >> 24) & 0xff;
  return;
}

unsigned long long int iPow(unsigned long long int base, int exp)
{
  if(exp == 0) return 1;
  if(base == 0) return 0;
  unsigned long long int res = 1;
  for(int i = 0; i < exp; i++)
  {
    if(res <= ULLONG_MAX / base)
    {
      res *= base;
    }
    else 
    {
      return 0;
    }
  }
  return res;
}

void bSortStr(char* buffer, int* index, size_t size, size_t* off)
{
  int temp;
  size_t len;
  if(!off[2]) len = strnlen(buffer, MAX_FIELD_LENGTH) + 1;
  else len = off[2];
  int check = 0;
  while(size > 1)
  {
    for(size_t j = 1; j < size; j++)
    {
      //This comparison must take into account the space between fields (not happening right now)
      check = strncmp(&buffer[index[j]*len + off[0]], &buffer[(index[j - 1])*len + off[0]], off[1]);
      if(check < 0)
      {
        temp = index[j - 1];
        index[j - 1] = index[j];
        index[j] = temp;
      }
    }
    size--;
  }
  return;
}

void bSortStr2(char* buffer, uint32_t* index, size_t size, size_t len)
{
  uint32_t temp;
  
  int check = 0;
  while(size > 1)
  {
    for(size_t j = 1; j < size; j++)
    {
      //This comparison must take into account the space between fields (not happening right now)
      check = strnicmp(&buffer[index[j]*len], &buffer[(index[j - 1])*len], len);
      if(check < 0)
      {
        temp = index[j - 1];
        index[j - 1] = index[j];
        index[j] = temp;
      }
    }
    size--;

  }
  return;
}

void zeroFill(char* sto, size_t size)
{
  for(size_t i = 0; i < size; i++)
  {
    if(sto[i] == ' ' || sto[i] == '\0') sto[i] = '0';
  }
}

void spaceFill(char *string, size_t size)
{
  for (size_t i = 0; i < size; i++)
  {
    if(string[i] == '\0')
    {
      string[i] = ' ';
    }
  }
  return;
}

void rightAlign(char *string, const size_t size)
{
  /* determine amount of spaces to the right */
  int check = 0;
  int strEnd = 0;

  for(size_t i = 0; i < size && !strEnd; i++)
  { 
    if(check == 0 && (string[i] != '\0' && string[i] != ' ')) 
      check = 1;
    if(check == 1 && (string[i] == '\0' || string[i] == ' ')) 
      strEnd = i;
  }

  if(strEnd)
  {
    memmove(&string[size - strEnd], string, strEnd);
    size_t i;
    for(i = 0; i < (size - strEnd); i++)
    {
      string[i] = ' ';
    }
    delLeadingZeroes(&string[i], size - i);
  }
  else
  {
    spaceFill(string, size);
  }
  return;
}

void delLeadingZeroes(char *string, size_t size)
{
  /* this assumes the number is right aligned to the size */
  {
    int i;
    for(i = 0; i < (size - 1) && (string[i] == '0' || string[i] == ' ') ; i++)
    {
      if(string[i] == '0' && string[i + 1] != '.')
        string[i] = ' ';
    }
  }
  return;
}

void rightAlignV2(char *string, const size_t size)
{
  /* determine amount of spaces to the right */
  int counter = 0;
  int check = 0;

  for(size_t i = 0; i < size; i++)
  { 
    if(check == 0 && (string[i] != '\0' && string[i] != ' ')) check = 1;
    if((string[i] == '\0' || string[i] == ' ') && check == 1) counter++;
    if(string[i] == '\0') string[i] = ' ';
  }
  if(counter > 0)
  {
    memmove(&string[counter], string, size - counter);

    for(int i = 0; i < counter; i++)
    {
      string[i] = ' ';
    }
  }
  return;
}
