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
#include "alarmLib.h"
#include "avr/EEPROM.h"
#include "rpcLib.h"
#include "tuls.h"
#include "byteStream.h"

Json *alarms;
bool setAlarm(Json* alarm) {

}
void checkAlarms() {
  p("checkAlarms\n");
  Json *alarm = alarms->child;
  while(alarm) {
    p("checking alarm\n");
    jsonPrintln(alarm, serialWriteByte);
    Json *namedArgs = jsonGet(alarm,2);
    int period = jsonGetInt(namedArgs,"period",0);
    int startTime = jsonGetInt(alarm,0,0);
    Json * script = jsonGet(alarm,1);
    Json * result = NULL;

    p("startTime=%d period=%d\n",startTime,period);
    p("%d >= %d\n",seconds(),startTime);

    if((seconds()-startTime)%period == 0) {

      p("triggering alarm");
      int repeat = jsonGetInt(namedArgs,"repeat",1);
      p("repeat = %d\n",repeat);
      if(repeat == 1 && period > 0) {
        p("trigger and delete\n");
        result = rpcEval(alarm,script);
        p("delete trigger\n");
        jsonDelete(alarms,alarm);
        p("trigger deleted");
      }
      else {
        p("trigger and save for later\n");
        result = rpcEval(alarm,jsonCopy(script));
        p("trigger done\n");
      }
      jsonPrintln(result,serialWriteByte);
      jsonDelete(NULL,result);
    }
    alarm = alarm->next;
  }
}
