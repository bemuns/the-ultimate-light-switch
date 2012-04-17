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
#include "rpcLib.h"
#include "rpcInterface.h"
#include "freeMemory.h"
#define LOCAL_DEBUG(x)
#define DEBUG(x) LOCAL_DEBUG(x)


void rpcDebug(char* type, int line) {
  p("RPC %s ERROR: line %d\n",type,line);
}

//TODO: input should be replaced with result and returned
//TODO: check inputs against json definition
//Returns a pointer to the return value, must be freed by the callee if not null
struct Json* rpcEval(Json *parent, Json *rpc) {
  DEBUG(p("%s(%x,%x)\n",__FUNCTION__,parent,rpc);jsonPrintln(rpc,serialWriteByte);)
  if (rpc == NULL) return NULL;
  if (rpc->type == JSON_OBJ) {
    //DEBUG(p("evaluating JSON_OBJ\n"));
    Json *id;
    union {
      Json *method;
      Json *result;
    };
    union {
      Json *params;
      Json *errors;
    };
    id = jsonGet(rpc,"id");
    if (id) {
      jsonDetach(rpc,id);
      DEBUG(jsonPrintln(id,serialWriteByte);)
    }

    method = jsonGet(rpc,"method");
    if (method) {

      params = jsonGet(rpc,"params");
      if (params) {
        DEBUG(jsonPrintln(method,serialWriteByte);)
      }

      //?jsonDetach(rpc,params);
      Json *rtn = rpcFunc(method->strVal,params);

      if(id) {

        Json *result = jsonNew(JSON_OBJ);
        Json *err;

        const char resultText[] = "result";
        const char errorText[] = "errors";

        if (rtn == NULL) {
          rtn = jsonNew(JSON_NULL);
          err = jsonNew(JSON_TRUE);
        }
        else {
          err = jsonNew(JSON_NULL);
        }

        rtn->name = (char *)resultText;
        err->name = (char *)errorText;

        jsonAppend(result,rtn);
        jsonAppend(result,err);
        jsonAppend(result,id);

        jsonSend(result);

        rtn->name = NULL;
        err->name = NULL;

        jsonDetach(result,rtn);

        jsonDelete(NULL,result);

      }

      rtn = jsonReplace(rpc,rtn);//Replace rpc so links to rpc still valid

      return rtn;

    }
    else if (result = jsonGet(rpc,"result")) {
      DEBUG(jsonPrintln(result,serialWriteByte);)
      errors = jsonGet(rpc,"errors");
      jsonPrintln(errors,serialWriteByte);
      jsonDelete(parent,rpc);
      return result;
    }
    else {
      DEBUG(p("non method object\n");jsonPrintln(rpc,serialWriteByte);)
    }
  }
  /*else if (rpc->type == JSON_ARRAY) {
    DEBUG(p("evaluating JSON_ARRAY\n"));
    Json * child = rpc->child;
    while(child) {
      Json * rtn = rpcEval(rpc,child);
      DEBUG(p("array rtn:");jsonPrintln(rtn,serialWriteByte);)
      DEBUG(p("array rpc:");jsonPrintln(rpc,serialWriteByte);)
      child = child->next;
      DEBUG(p("next=%x\n",child);)
    }
    //jsonDelete(parent,rpc);
    return rpc;
  }*/
  //DEBUG(p("evaluating %d\n",rpc->type));
  return rpc;
}

