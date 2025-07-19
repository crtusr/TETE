#ifndef DBFTOOL_H
#define DBFTOOL_H

#include <inttypes.h>

#define FIELD_SIZE 32L
#define FIELD_NAME 11L
#define FIELD_TYPE 1L
#define RESERVED_BYTES 4L
#define FIELD_LENGTH 1L
#define DECIMAL_COUNT 1L
#define WORK_AREA 2L
#define EXAMPLE 1L
#define RESERVED_BYTES2 10L
#define INDEX_FLAG 1L
#define MAX_DBF_FIELDS 255
/*
#define DBF_VER 1L
#define DATE 3L
#define N_OF_RECORDS 1L
#define HEADER_BYTES 4L
#define RECORD_BYTES 1L
#define H_RES_1 2L
#define INC_TRANS 1L
#define ENCR_FLAG 1L
#define H_RES_2 12L
#define LANG_DRIV_ID 1L
#define H_RES_3 2L
*/
#define TERMINATOR 0x0D
#define MAX_FIELD_LENGTH 254

#define MEMOBLOCKSIZE 512


typedef struct {
	uint8_t		dbfversion; 	// 1
	uint8_t		date[3];	// 3
	uint32_t	nofrecords;	// 4
	uint16_t 	header_bytes;
	uint16_t	record_bytes;
	uint16_t	res1;
	uint8_t		inctransaction;
	uint8_t		encryption;
	uint8_t		res2[12];
	uint8_t		mdxflag;
	uint8_t		langdrivID;
	uint16_t	res3;
} header;

typedef struct {
	char		fieldname[11];
	char		type;
	char		res1[4];
	uint8_t		length;
	uint8_t 	decimal;
	uint16_t 	workID;
	uint8_t 	example;
	char		res2[10];
	uint8_t 	index_flag;
} descriptor;


void store_header_data(header *head, FILE *file, int i);
void store_descriptor_data(descriptor *fields, FILE *file);
int get_index(char* campo, char* string, FILE* file, header* head, descriptor* descr);
int get_data(char* buffer, int indice, char* campo, FILE* file, header* head, descriptor* descr);
int get_record(char* buffer, int indice, FILE* file, header* head, descriptor* descr);
void rightAlign(char *string, size_t size);
int readMemo(unsigned char* buffer, char* file, const int block);
int replaceField();
void dateFormatter(char* date);
long long atonum(const char* number);
void spaceFill(char *string, size_t size);
int sumFields(char *res, char* a, char* b);
int subFields(char *res, char* a, char* b);
int addDecimals(char *string, const size_t size, const size_t decimals);
int addRecord(char* buffer, char* fname, size_t size);
int addMemo(const char* fileName, char* buffer);
int replaceMemo(const char* fileName, char* buffer, int blockNum);

#endif
