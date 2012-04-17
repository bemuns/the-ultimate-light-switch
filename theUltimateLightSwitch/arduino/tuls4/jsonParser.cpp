/*******************************************************************************
 * Copyright (c) 2012 Robert Barnes.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Public License v3.0
 * which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/gpl.html
 * 
 * Contributors:
 *     Robert Barnes - initial API and implementation
 ******************************************************************************/

#include "jsonLib.h"
#include <math.h>
#include "tuls.h"

#define DEBUG(x)

static struct Json * jsonParseObj();
static struct Json * jsonParseArray();
static struct Json * jsonPareInt();
static struct Json * jsonPareDouble();
static struct Json * jsonParseStr();
static struct Json * jsonParseNumber();
static struct Json* jsonParseTrue();
static struct Json* jsonParseFalse();
static struct Json* jsonParseNull();
static void jsonParseError(int);
static char * parseStr(char*, int);
static long parseInt(char*, int);


static byte (*readByte)(void);
static byte (*peekByte)(void);

#define jsonParseGetAssert(c) {if ((c)!=jsonGetc()) {jsonParseError(__LINE__); return NULL;}}
#define jsonParsePeekAssert(c) {if ((c)!=jsonPeekc()){jsonParseError(__LINE__); return NULL;}}

static char jsonBuf[JSON_BUF_SIZE];

static char prevGetc = 0;
static char currGetc = 0;


static char jsonPeekc() {
  DEBUG(Serial.println(__FUNCTION__);)
  while (1) {
    char c = peekByte();
    if (c != '\\' &&
       (c == '\n' || c == ' ' ||
        c == '\r' || c == '\t'))  {
          readByte();
          continue;
        }
    else  {
      return c;
    }
  }
}

static char jsonGetc() {
  DEBUG(Serial.println(__FUNCTION__);)
  prevGetc = currGetc;
  while (1) {
    currGetc = readByte();
    if (prevGetc != '\\' &&
       (currGetc == '\n' || currGetc == ' ' ||
        currGetc == '\r' || currGetc == '\t'))  {
          readByte();
          continue;
        }
    else  {
      return currGetc;
    }
  }

}

#if 0
static void jsonParseError(char expected, char got) {
  Serial.print("ERROR: expecting ");
  Serial.print(expected);
  Serial.print(" got ");
  Serial.print(got);
  Serial.print("\n");
}
#endif

static void jsonParseError(int line) {
  jsonError("PARSE",line);
}

static boolean isDigit(char c) {
  DEBUG(Serial.println(__FUNCTION__);)
  return (c >= '0' && c <= '9');
}


/*Json *jsonParse(const char * str, int size) {
  inputStr = str;
  inputStrEnd = inputStr+size;
  readByte = strReadByte;
  peekByte = strPeekByte;
  jsonParse();
}*/

Json *jsonParse(byte (*newPeekByte)(void), byte (*newReadByte)(void)) {
  DEBUG(Serial.println(__FUNCTION__);)
  readByte = newReadByte;
  peekByte = newPeekByte;
  Json *json = jsonParse();
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return json;
}

int freeMemory();

Json *jsonParse() {
  DEBUG(Serial.println(__FUNCTION__);)
  DEBUG(p("Free Memory = %u\n",freeMemory());)
  char c = jsonPeekc();
  switch(c) {
    case '{':
      return jsonParseObj();
    case '\"':
      return jsonParseStr();
    case '[':
      return jsonParseArray();
    case 't':
    	return jsonParseTrue();
    case 'n':
    	return jsonParseNull();
    case 'f':
    	return jsonParseFalse();
    default:
      if (isDigit(c) || c == '-') {
        DEBUG(Serial.print("found number\n");)
        return jsonParseNumber();
      }
      jsonParseError(__LINE__);
      return NULL;
  }
}  

static struct Json* jsonParseTrue() {
  DEBUG(Serial.println(__FUNCTION__);)
	jsonParseGetAssert('t');
	jsonParseGetAssert('r');
	jsonParseGetAssert('u');
	jsonParseGetAssert('e');
	return jsonNew(JSON_TRUE);
}
static struct Json* jsonParseFalse() {
  DEBUG(Serial.println(__FUNCTION__);)
	jsonParseGetAssert('f');
	jsonParseGetAssert('a');
	jsonParseGetAssert('l');
	jsonParseGetAssert('s');
	jsonParseGetAssert('e');
	return jsonNew(JSON_FALSE);
}
static struct Json* jsonParseNull() {
  DEBUG(Serial.println(__FUNCTION__);)
	jsonParseGetAssert('n');
	jsonParseGetAssert('u');
	jsonParseGetAssert('l');
	jsonParseGetAssert('l');
	return jsonNew(JSON_NULL);
}

static Json* jsonParseNumber() {
  DEBUG(Serial.println(__FUNCTION__);)
  boolean neg = false;
  if (jsonPeekc() == '-') {
    neg = true;  
    jsonGetc();
  }
  int i = 0;
  int dblDot = 0;
  char c = jsonPeekc();
  while(1) {
    if (isDigit(c)) {
      jsonBuf[i++] = c -'0';
    }
    else if (c == '.') {
      if(dblDot) {
        DEBUG(Serial.println("ERROR: double .");)
        jsonParseError(__LINE__);
        return NULL;
      }
      dblDot = i;
    } 
    else {
      break;
    }
    jsonGetc();
    c = jsonPeekc();
  } 
  Json *number = NULL;
  if (dblDot == 0) {
    long value = parseInt(jsonBuf,i);
    if (neg) value = -value;
    number = jsonNewInt(value);
    DEBUG(p("int value = ");Serial.println(value);)
  } 
  else {
    double value = (double)parseInt(jsonBuf,dblDot);
    value += (double)parseInt(jsonBuf+dblDot,i-dblDot)/pow(10,i-dblDot);
    if (neg) value = -value;
    number = jsonNewDouble(value);
  }
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return number;
}

static long parseInt(char* buf, int len) {
  DEBUG(Serial.println(__FUNCTION__);)
  unsigned long tens = 1, value = 0;
  for(int i=len-1; i>=0; i--) {
    value += buf[i]*tens;
    tens *= 10;
  }
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return value;
}


static Json* jsonParseObj() {
  DEBUG(Serial.println(__FUNCTION__);)
  jsonParseGetAssert('{');
  Json * obj = jsonNew(JSON_OBJ);
  
  Json *prev = NULL;  
  while (jsonPeekc() != '}') {
    if(prev) {
      jsonParseGetAssert(',');
    }
    char *name = parseStr(jsonBuf,JSON_BUF_SIZE);
    DEBUG(
    Serial.print(name);Serial.println(':');
    )
    jsonParseGetAssert(':');
    Json *curr = jsonParse();
    if (curr == NULL) {
      jsonParseError(__LINE__);
      DEBUG(Serial.println(name);)
      return NULL;
    }
    curr->name = name;
    //curr->prev = prev;
    if (prev) {
      prev->next = curr;
    }
    else {
      obj->child = curr;
    }
    prev = curr;
  }
  jsonParseGetAssert('}');
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return obj; 
}

static Json* jsonParseArray() {
  DEBUG(Serial.println(__FUNCTION__);)
  jsonParseGetAssert('[');
  Json * array = jsonNew(JSON_ARRAY);
  int index = 0;
  Json *prev = NULL;  
  while (jsonPeekc() != ']') {
    if(prev) {
      jsonParseGetAssert(',');
    }
    Json *curr = jsonParse();
    if (curr == NULL) {
      jsonParseError(__LINE__);
      return NULL;
    }
    //curr->index = index++;
    //curr->prev = prev;
    if (prev) {
      prev->next = curr;
    }
    else {
      array->child = curr;
    }
    prev = curr;
  }
  jsonParseGetAssert(']');
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return array; 
}

static Json* jsonParseStr() {
  DEBUG(Serial.println(__FUNCTION__);)
  Json * jsonStr = jsonNew(JSON_STR);
  jsonStr->strVal = parseStr(jsonBuf,JSON_BUF_SIZE);
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return jsonStr;
}

static char * parseStr(char* tmpBuf, int maxSize) {
  DEBUG(Serial.println(__FUNCTION__);)
  jsonParseGetAssert('\"');
  char c;
  char prev;
  int i = 0;
  while (!(jsonGetc() == '\"' && prevGetc != '\\')) {
    tmpBuf[i++] = currGetc;
    if (i == maxSize) {
      DEBUG(Serial.print("ERROR: string longer than");Serial.println(maxSize);)
      jsonParseError(__LINE__);
      return NULL;
    }
  }
  tmpBuf[i++] = 0;
  char *str = (char *)malloc(i);
  if (!str) {
    DEBUG(Serial.println("ERROR: out of memory");)
    jsonParseError(__LINE__);
    return NULL;
  }
  memcpy(str,tmpBuf,i);
  DEBUG(Serial.println(str);)
  DEBUG(p("%s returning\n",__FUNCTION__);)
  return str;
}
