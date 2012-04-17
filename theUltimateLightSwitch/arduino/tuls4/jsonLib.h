#-------------------------------------------------------------------------------
# Copyright (c) 2012 Robert Barnes.
# All rights reserved. This program and the accompanying materials
# are made available under the terms of the GNU Lesser Public License v2.1
# which accompanies this distribution, and is available at
# http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
# 
# Contributors:
#     Robert Barnes - initial API and implementation
#-------------------------------------------------------------------------------
#ifndef JSONLIB_H
#define JSONLIB_H

#include "tuls.h"

#define JSON_BUF_SIZE 128

enum JsonType {JSON_STR,JSON_INT,JSON_LONG,JSON_DOUBLE,JSON_OBJ,JSON_ARRAY,JSON_TRUE,JSON_FALSE,JSON_NULL};
//enum JsonState {JSON_ATTACHED,JSON_DETACHED,JSON_DELETED};

//#define jsonGetType(json) (json->properties&0x0F)
//#define jsonGetState(json) ((json->properties&0x30)>>4)
//#define jsonNamed(properties) (properties&0x80)

//#define jsonSetType(json,type) (json->properties ^= type^jsonGetType(json))
//#define jsonSetState(json,state) (json->properties ^= (state^jsonGetState(json))<<4)
//#define jsonNamed(properties) (properties&0x80)

//#define jsonIsValid(json) ((json)!=NULL && jsonGetState((json)->properties) == JSON_ATTACHED)

typedef struct Json {
  byte type;
  struct Json *next;
  union {
    char *strVal;
    int intVal;
    long *longPtr;
    double *doublePtr;
    struct Json *child;
  };
  union {
	  char *name;
	  int index;
  };
} Json;


//parsing
struct Json * jsonParse();
struct Json * jsonParse(const char * str, int size);
struct Json * jsonParse(byte (*newPeekByte)(void), byte (*newReadByte)(void));

//printing
void jsonPrint(Json * json, void (*newWriteByte)(byte));
void jsonPrintln(Json * json, void (*newWriteByte)(byte));

//creation & deletion
struct Json * jsonNew(byte type);
boolean jsonDelete(Json * parent, Json * josn);
struct Json * jsonCopy(Json * src);
struct Json * jsonCopyChildren(Json *src);
struct Json * jsonAppend(Json * parent, Json * item);
boolean jsonDeleteChildren(Json * parent);
struct Json * jsonDetach(Json * parent, Json * json);
struct Json * jsonGet(Json * parent,char *);
struct Json * jsonGet(Json * parent,int);
int jsonGetInt(Json *obj, int index, int defaultValue);
int jsonGetInt(Json *obj, char* name, int defaultValue);
void jsonSend(Json * msg);
struct Json* jsonReplace(Json *oldJson, Json *newJson);
void jsonError(char* type, int line);
struct Json * jsonNewInt(long value);
struct Json * jsonNewDouble(double value);
#endif
