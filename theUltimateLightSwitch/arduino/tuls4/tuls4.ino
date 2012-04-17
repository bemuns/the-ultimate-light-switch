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
#include "deviceInfo.h"
#include "byteStream.h"
#include "rpcLib.h"
#include "jsonLib.h"
#include "tuls.h"
#include "eepromLib.h"
#include "freeMemory.h"

#define DEBUG(x)
int freeMemory();


void p(char *fmt, ... ){
        char tmp[128]; // resulting string limited to 128 chars
        va_list args;
        va_start (args, fmt );
        vsnprintf(tmp, 128, fmt, args);
        va_end (args);
        Serial.print(tmp);
}


void checkSerial() {
  if (Serial.available()!=0) {
    int c = Serial.read();
    if (c == MAGIC_START) {
      //p("Start\n");

      DEBUG(Serial.println("###parse");)
      Json * msg = jsonParse(serialPeekByte,serialReadByte);
      DEBUG(Serial.println("***parse");)
      DEBUG(printFreeMemory();)

      DEBUG(Serial.println("###print");)
      DEBUG(jsonPrintln(msg, serialWriteByte);)
      DEBUG(Serial.println("***print");)
      DEBUG(printFreeMemory();)

      DEBUG(Serial.println("###process");)
      rpcEval(NULL,msg);
      DEBUG(Serial.println("***process");)
      DEBUG(printFreeMemory();)

      /*DEBUG(Serial.println("###return");)
      jsonPrintln(msg,serialWriteByte);
      DEBUG(Serial.println("***return");)
      DEBUG(printFreeMemory();)*/

      DEBUG(Serial.println("###delete");)
      jsonDelete(NULL,msg);
      DEBUG(Serial.println("***delete");)
      DEBUG(printFreeMemory();)
    }
    else if(c == MAGIC_STOP) {
      p("Stop\n");
    }
    else {
      p("Invalid byte: %x %c\n",c,c);
    }
  }
}


void setup(void) {
  Serial.begin(57600);
  Serial.flush();
  p("id = %u\n",eepromGetId());

#if 0
  DEBUG(p("1");printFreeMemory();)
  void *x = malloc(288);
  DEBUG(p("2");printFreeMemory();)
  free(x);
  DEBUG(p("3");printFreeMemory();)

  char s[] = "{\"m\":\"add\",\"p\":[1,{\"m\":\"add\",\"p\":[-1.0,-88000]}],\"i\":99}";
  setInputStr(s,sizeof(s));
  Json * tmp = jsonParse(ramPeekByte,ramReadByte);
  DEBUG(p("3");printFreeMemory();)
  jsonDelete(NULL,tmp);
  DEBUG(p("4");printFreeMemory();)
#endif


  DEBUG(printFreeMemory();)
#if 0
  setInputStr(deviceLocationStr,strlen(deviceLocationStr));
  Json * locationInfo = jsonParse(flashPeekByte,flashReadByte);
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  jsonPrintln(locationInfo);
  jsonDelete(NULL,locationInfo);
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());

  setInputStr(deviceSensorsStr,strlen(deviceSensorsStr));
  Json * sensorInfo = jsonParse(flashPeekByte,flashReadByte);
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
  jsonPrintln(sensorInfo);
  jsonDelete(NULL,sensorInfo);
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
#endif

#if 0
  eepromFreeAllPages();
  Serial.println(PAGES_OFFSET);
  void * x = eepromMalloc(255);
  Serial.println((int)x);
  void * y = eepromMalloc(10);
  Serial.println((int)y);
  eepromFree(x);
  eepromFree(y);
  x = eepromMalloc(127);
  Serial.println((int)x);
  eepromFree(x);
#endif


}

void loop(void) 
{

  checkSerial();
#if 0
  checkAlarms();
#endif
  //delay(1000);
  //Serial.print(fgetc(stdin));
}
