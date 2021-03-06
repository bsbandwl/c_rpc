#ifndef __RMIPARSE_H__
#define __RMIPARSE_H__

#ifdef _WIN32
#define snprintf _snprintf
#define strdup _strdup
#define fileno _fileno
#endif

#include "list.h"
#include "crc32.h"

#define LIST_MAX_TIME	5000	// unit: ms
#define LIST_MAX_PACKET	0
#define LIST_MAX_USER	16

/*#define BUF_SIZE		(1*1024*1024)*/
#define BUF_SIZE		(0)

#define PARA_IN		1
#define PARA_OUT	2

struct parameter {
	char name[64];	// para_name
	char type[64];	// unsigned int
	int pointer;
	int len;
	int field_num;
	int field_type;
	int string;
	int dir;
	int para_num;
};

struct struct_info {
	char name[64];	// aaa
	char type[64];	// struct, union
	LIST_S para_list;
};

struct newtype_info {
	char new_type[64];
	char orig_type[64];
	char orig_name[64];
};

struct func_info {
	char ret_type[64];
	int pointer;
	char func_name[64];
	unsigned int func_id;
	LIST_S para_list;
};

extern int yylineno;
extern int yylex (void);
extern void yyerror(char *);

extern LIST_S g_struct_list;
extern LIST_S g_func_list;
extern LIST_S g_newtype_list;

int gen_field_type(struct parameter * para);

#endif
