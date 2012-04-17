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
#include "tuls.h"
#define DEBUG(x)

static void jsonPrintObj(Json *);
static void jsonPrintArray(Json *);
static void printStr(char*);
static void puts(char*);
static void jsonPrint(Json * json);


static void (*writeByte)(byte);
#define putc writeByte

static void puts (char * str) {
  DEBUG(p("%s(%x)\n",__FUNCTION__,str);)
  if (str == NULL) return;
  while(*str != '\0') putc(*(str++));
}

static void printStr(char * str) {
  DEBUG(p("%s(%x)\n",__FUNCTION__,str);)
  if(str == NULL) return;
  putc('\"');
  puts(str);
  putc('\"');
}

void jsonPrint(Json * json, void (*newWriteByte)(byte)) {
  DEBUG(p("%s\n",__FUNCTION__);)
  writeByte = newWriteByte;
  jsonPrint(json);
}

void jsonPrintln(Json * json, void (*newWriteByte)(byte)) {
  DEBUG(p("%s\n",__FUNCTION__);)
  writeByte = newWriteByte;
  jsonPrint(json);
  putc('\n');
}


static void jsonPrintObj(Json * obj) {
  DEBUG(p("%s\n",__FUNCTION__);)
  if (obj == NULL) return;
  putc('{');
  Json* child = obj->child;
  while(child) {
    if(child != obj->child) putc(',');
    jsonPrint(child);
    child = child->next;
  }
  putc('}');
}
static void jsonPrintArray(Json * array) {
  DEBUG(p("%s\n",__FUNCTION__);)
  if (array == NULL) return;
  putc('[');
  Json* child = array->child;
  while(child) {
    if(child != array->child) putc(',');
    jsonPrint(child);
    child = child->next;
  }
  putc(']');
}

static void jsonPrint(Json * json) {
  DEBUG(p("%s\n",__FUNCTION__);)
  if(json == NULL) return;
  if((int)json->name >= 256){
    printStr(json->name);
    putc(':');
  }
  switch (json->type) {
    case JSON_OBJ:
      jsonPrintObj(json);
      break;
    case JSON_STR:
      printStr(json->strVal);
      break;
    case JSON_ARRAY:
      jsonPrintArray(json);
      break;
    case JSON_INT:
      Serial.print(json->intVal);
      break;
    case JSON_DOUBLE:
      Serial.print(*(json->doublePtr));
      break;
    case JSON_LONG:
    	Serial.print(*(json->longPtr));
    	break;
    case JSON_TRUE:
      Serial.print("true");
      break;
    case JSON_FALSE:
      Serial.print("false");
    	break;
    case JSON_NULL:
      Serial.print("null");
    break;
    default:
      Serial.println("ERROR: unknown JSON type");
  }
}

void jsonSend(Json * msg) {
  DEBUG(p("%s\n",__FUNCTION__);)
  Serial.write(MAGIC_START);
  jsonPrint(msg,serialWriteByte);
  Serial.write(MAGIC_STOP);
}
