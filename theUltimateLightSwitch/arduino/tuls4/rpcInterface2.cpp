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
//This file is generated automatically, do not edit
#include <string.h>
#include "jsonLib.h"
#include "rpcInterface.h"
#include "eepromLib.h"

#define DEBUG(x) x

/*static int sum(char * name) {
    int sum = 0;
    while(*name) {sum += *(name++);}
    return sum;
}
Json * rpcFunc(char* method, Json * params) {
switch(sum(method)) {
case 0x129: return rpc_add(params);
case 0x20d: return rpc_alarm(params);
case 0x1ed: return rpc_getId(params);
default: return NULL;
}
}*/

int digitalToggle(int pin) {
  int state = digitalRead(pin);
  //p("digitalToggle(%d) %d -> %d\n");
  digitalWrite(pin,!state);
  return digitalRead(pin);
}

int getId() {
  return eepromGetId();
}

Json * rpcFunc(char* method, Json * params)
{
  //DEBUG(p("%s(%s)\n",__FUNCTION__,method));
  if(0){}
  else if(strcmp(method,"digitalToggle")==0) {
      Json *curr = params->child;
      if (curr==NULL || curr->type!=JSON_INT) return NULL;
      unsigned char arg0 = curr->intVal;
      //p("digitalWrite(%d,%d)\n",arg0,arg1);
      int rtn = digitalToggle(arg0);
      return jsonNewInt(rtn);
  }
  else if(strcmp(method,"digitalWrite")==0) {
    Json *curr = params->child;
    if (curr==NULL || curr->type!=JSON_INT) return NULL;
    unsigned char arg0 = curr->intVal;
    curr = curr->next;
    if (curr==NULL || curr->type!=JSON_INT) return NULL;
    unsigned char arg1 = curr->intVal;
    //p("digitalWrite(%d,%d)\n",arg0,arg1);
    digitalWrite(arg0,arg1);
    return jsonNew(JSON_OBJ);//void;
  }
  else if(strcmp(method,"digitalRead")==0) {
    Json *curr = params->child;
    if (curr==NULL || curr->type!=JSON_INT) return NULL;
    unsigned char arg0 = curr->intVal;
    unsigned char rtn = digitalRead(arg0);
    return jsonNewInt(rtn);
  }
  else if(strcmp(method,"getId")==0) {
    int rtn = getId();
    return jsonNewInt(rtn);
  }
  else if(strcmp(method,"pinMode") == 0) {
    Json *curr = params->child;
    if (curr==NULL || curr->type!=JSON_INT) return NULL;
    unsigned char arg0 = curr->intVal;
    if (curr==NULL || curr->type!=JSON_INT) return NULL;
    unsigned char arg1 = curr->intVal;
    pinMode(arg0,arg1);
    return jsonNew(JSON_OBJ);
  }
  return NULL;
}

