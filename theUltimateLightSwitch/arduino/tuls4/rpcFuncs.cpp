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

#include "rpcLib.h"
#include "byteStream.h"
#include "tuls.h"
#include "eepromLib.h"
#define DEBUG(x)

Json * rpc_add(Json * params) {
  DEBUG(p("%s\n",__FUNCTION__);)
  long longSum=0;
  double doubleSum=0;

  bool resultIsDouble = false;
  Json * param = params->child;
  while(param) {
    DEBUG(jsonPrintln(param,serialWriteByte);)
    Json * next = param->next;

    param = rpcEval(params,param);

    if (!resultIsDouble) {
      if(param->type == JSON_INT) {
        longSum += param->intVal;
      }
      else if (param->type == JSON_LONG) {
        longSum += *(param->longPtr);
      }
      else if (param->type == JSON_DOUBLE) {
        doubleSum = longSum;
        resultIsDouble = true;
      }
      else {
        rpcDebug("",__LINE__);
        return jsonNew(JSON_NULL);
      }
    }
    if (resultIsDouble) {
      if(param->type == JSON_INT) {
        doubleSum += param->intVal;
      }
      else if (param->type == JSON_LONG) {
        doubleSum += *(param->longPtr);
      }
      else if (param->type == JSON_DOUBLE) {
        doubleSum += *(param->doublePtr);
      }
      else {
        rpcDebug("",__LINE__);
        return jsonNew(JSON_NULL);
      }
    }
    param = next;
  }
  if (!resultIsDouble) {
    return jsonNewInt(longSum);
  }
  else {
    return jsonNewDouble(doubleSum);
  }
}

Json * rpc_getId(Json * params) {
  return jsonNewInt(eepromGetId());
}

extern Json * alarms;

Json * rpc_alarm(Json * params) {

  //setAlarm(params);

	return jsonNew(JSON_TRUE);
}
