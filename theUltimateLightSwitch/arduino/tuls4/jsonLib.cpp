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
#include "jsonLib.h"
#include "tuls.h"
#include <string.h>
#include "byteStream.h"

#define LOCAL_DEBUG(x)
#define DEBUG(x) LOCAL_DEBUG(x)

static boolean jsonDelete(Json *json);

void jsonError(char* type, int line) {
  p("JSON %s ERROR: line %d\n",type,line);
}

static void jsonMemoryError(int line) {
  jsonError("MEMMORY",line);
}

struct Json * jsonNewInt(long value) {
  DEBUG(p("%s(%x)\n",__FUNCTION__,value));
  Json * number;
  if (value >= (((long)2)<<15) || value < -(((long)2)<<15)) {
    DEBUG(p("creating a long\n");)
    number = jsonNew(JSON_LONG);
    *(number->longPtr) = value;
  }
  else {
    DEBUG(p("creating an int\n");)
    number = jsonNew(JSON_INT);
    number->intVal = (int)value;
  }
  return number;
}

struct Json * jsonNewDouble(double value) {
  DEBUG(p("%s(%x)\n",__FUNCTION__,value));
  Json *number = jsonNew(JSON_DOUBLE);
  *(number->doublePtr) = value;
  return number;
}

struct Json * jsonNew(byte type) {

  DEBUG(p("%s(%x)\n",__FUNCTION__,type));

	/*Json * json;
	int mallocSize;
	switch(type) {
	case JSON_OBJ:
	case JSON_OBJ:
	case JSON_INT:
	case JSON_DOUBLE:
		mallocSize = sizeof(Json);
		break;
	}*/
	Json * json = (Json *)malloc(sizeof(Json));
	if (!json) {
	  jsonMemoryError(__LINE__);
	  return NULL;
	}
	json->next =  NULL;
	json->name = NULL;
	if (type == JSON_DOUBLE || type == JSON_LONG) {
		json->doublePtr = (double *)malloc(sizeof(double));
		if (!json->doublePtr) {
		  jsonMemoryError(__LINE__);
		  return NULL;
		}
	} else {
		json->intVal = 0;
	}
	json->type = type;
	//jsonSetType(type);
	//jsonSetState(JSON_ATTACHED);
	return json;
}

struct Json * jsonSearch(Json *parent, Json *needle) {
	if(!needle || !parent || !parent->child) return NULL;
	DEBUG(p("%s(%x,%x)\n",__FUNCTION__,parent,needle));
	Json *prev = parent;
	Json *curr = parent->child;
	while(curr) {
		if(curr == needle) return prev;
		prev = curr;
		curr = curr->next;
	}

	curr = parent->child;
	while(curr) {
		prev = jsonSearch(curr,needle);
		if(prev) return prev;
		curr = curr->next;
	}
	return NULL;
}

struct Json *jsonDetach(Json *parent, Json *json) {
  if(!parent || !json) return NULL;
  DEBUG(p("%s(%x,%x)\n",__FUNCTION__,parent,json));
	Json *prev = jsonSearch(parent,json);
	if(prev) {
		if (prev->next == json) {
			prev->next = json->next;
		}
		else if(prev->child == json) {
			prev->child = json->next;
		}
		else {
		  jsonError("",__LINE__);
			return NULL;
		}
		//jsonDeleteChildren(json);
		return json;
	}
	return NULL;
}


boolean jsonDeleteChildren(Json * parent) {
	if(parent == NULL) return false;
	DEBUG(p("%s(%x)\n",__FUNCTION__,parent));
  Json * child = parent->child;
	switch(parent->type) {
	case JSON_OBJ:
	case JSON_ARRAY:
    while(child) {
      Json *next = child->next;
      jsonDelete(child);
      child = next;
    }
    parent->child = NULL;
    break;
	case JSON_LONG:
	case JSON_STR:
	case JSON_DOUBLE:
	  free(child);
	  break;
	default:
	  return false;
	}
	return true;
}



struct Json* jsonCopyChildren(Json *src, Json *dst) {
  if(src == NULL || dst == NULL ||
    !(src->type == JSON_OBJ || src->type == JSON_ARRAY) ||
    !(dst->type == JSON_OBJ || src->type == JSON_ARRAY)
    ) return NULL;

  DEBUG(p("%s(%x,%x)\n",__FUNCTION__,src,dst));

  Json * srcChild = src->child;
  Json * dstChild = dst->child = jsonCopy(srcChild);

  while(dstChild = dstChild->next = jsonCopy(srcChild = srcChild->next));

  return dstChild;
}
struct Json* jsonCopy(Json *src) {
  if (src == NULL) return NULL;
  DEBUG(p("%s(%x)\n",__FUNCTION__,src));
  Json *dst = jsonNew(src->type);
  if ((int)src->name >= 256) {
    dst->name = (char *)malloc(strlen(src->name)+1);
    if (!dst->name) {
      jsonMemoryError(__LINE__);
      return NULL;
    }
    strcpy(dst->name,src->name);
  }
  switch(src->type) {
  case JSON_OBJ:
  case JSON_ARRAY:
    jsonCopyChildren(src,dst);
    break;
  case JSON_STR:
    dst->strVal = (char *)malloc(strlen(src->strVal)+1);
    if (!dst->strVal) {
      jsonMemoryError(__LINE__);
      return NULL;
    }
    strcpy(dst->strVal,src->strVal);
    break;
  case JSON_DOUBLE:
  case JSON_LONG:
    dst->doublePtr = (double *)malloc(sizeof(double));
    if (!dst->doublePtr) {
      jsonMemoryError(__LINE__);
      return NULL;
    }
    *(dst->doublePtr) = *(src->doublePtr);
    break;
  default:
    dst->intVal = src->intVal;
  }
  return dst;
}

struct Json* jsonReplace(Json *oldJson, Json *newJson) {
#define LOCAL_DEBUG(x)
  DEBUG(p("%s(%x,%x)\n",__FUNCTION__,oldJson,newJson));
  if (oldJson == NULL || newJson == NULL) return oldJson;

  jsonDeleteChildren(oldJson);

  if((int)newJson->name >= 256) {
    free(newJson->name);
  }

  oldJson->type = newJson->type;
  oldJson->intVal = newJson->intVal;

  free(newJson);

  return oldJson;
#define LOCAL_DEBUG(x)
}

//This will not fix links from parent or siblings
static boolean jsonDelete(Json *json) {
  if (json == NULL) return false;

  DEBUG(p("%s(%x)\n",__FUNCTION__,json));

  if ((int)json->name >= 256) {
    DEBUG(p("free(%x|%s)\n",(int)json->name,json->name));
    free(json->name);
  }

  jsonDeleteChildren(json);

  free(json);

  return true;
}

boolean jsonDelete(Json *parent, Json *json){
	if(json==NULL) return false;
	DEBUG(p("%sParent(%x,%x)\n",__FUNCTION__,parent,json);jsonPrintln(json,serialWriteByte);)
	if (parent) {
	  DEBUG(Serial.println("parent"));
		if (jsonDetach(parent,json)==NULL) return false;
	}
	DEBUG(Serial.println("delete self"));
	jsonDelete(json);
}

struct Json * jsonGet(Json * obj, char* name) {
	if(obj == NULL || name == NULL) return NULL;
	DEBUG(p("%s(%x,%x)\n",__FUNCTION__,obj,name));
	Json * item = obj->child;
	while(item) {
	  DEBUG(p("strcmp(%s,%s)=%d\n",item->name,name,strcmp(item->name,name)));
		if (strcmp(item->name,name) == 0) {
			break;
		}
		item = item->next;
	}
	return item;
}

struct Json * jsonGet(Json * array, int index) {
	if(array == NULL) return NULL;
	DEBUG(p("%s(%x,%x)\n",__FUNCTION__,array,index));
	int count = 0;
	Json * item = array->child;
	while(item && count++ != index) {
		item = item->next;
	}
	return item;
}

int jsonGetInt(Json *obj, char *name, int defaultValue) {
  if (obj == NULL || name == NULL) return defaultValue;
  Json * rtn = jsonGet(obj,name);
  if(rtn) return rtn->intVal;
  else return defaultValue;
}

int jsonGetInt(Json *obj, int index, int defaultValue) {
  if (obj == NULL) return defaultValue;
  Json * rtn = jsonGet(obj,index);
  if(rtn) return rtn->intVal;
  else return defaultValue;
}

struct Json * jsonAppend(Json * parent, Json * item) {
  if (parent == NULL || item == NULL) return NULL;
  DEBUG(p("%s(%x,%x)\n",__FUNCTION__,parent,item));
  if (parent->type == JSON_OBJ) {
    item->next = parent->child;
    parent->child = item;
    return item;
  }
  else if (parent->type == JSON_ARRAY) {
    Json *child = parent->child;
    if (child) {
      while(child->next) child = child->next;
      child->next = item;
      item->index = child->index + 1;
    }
    else {
      parent->child = item;
      item->index = 0;
    }
  }
  else {
    DEBUG(Serial.print("ERROR: append expecting an object or array");)
    jsonError("",__LINE__);
    return NULL;
  }
}
/*
int jsonArraySize(Json* array) {
  if(array == NULL) return 0;
  DEBUG(p("%s(%x)\n",__FUNCTION__,array));
  int size = 0;
  Json * child = array->child;
  while(child) {
    switch(child->type) {
      case JSON_OBJ:
      case JSON_ARRAY:
      case JSON_LONG:
      case JSON_STR:
      case JSON_DOUBLE:
        size += sizeof(long);
        break;
      case JSON_INT:
      case JSON_TRUE:
      case JSON_FALSE:
      case JSON_NULL:
        size += sizeof(int);
        break;
    }
    child = child->next;
  }
  return size;
}
*/



