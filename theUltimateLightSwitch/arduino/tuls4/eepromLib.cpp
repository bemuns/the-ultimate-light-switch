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
#include "eepromLib.h"
#include <avr/EEPROM.h>
#include "tuls.h"
#define DEBUG(x)

typedef struct PageEntry {
  bool used  : 1;
  bool first : 1;
}PageEntry;

#define EMPTY   0
#define FIRST   1
#define LAST    2
#define FILLED  3
void* eepromMalloc(int size) {
  DEBUG(p("%s(%d)\n",__FUNCTION__,size);)
  byte b;
  int block = 0, first = 0;
  for(int i=0; i<NUM_PAGES; i++) {
    if(i%4 == 0) b = eeprom_read_byte((uint8_t *)PAGE_TABLE_OFFSET + i/4);
    DEBUG(p("b = %x\n",b);)
    byte entry = (b >> ((i%4)*2))&0x03;
    DEBUG(p("entry = %x\n",entry);)
    if(entry == EMPTY) {
      DEBUG(p("EMPTY\n");)
      if (block == 0) {
        first = i;
      }
      block += 1;
    }
    else {
      DEBUG(p("NOT EMPTY\n");)
      block = 0;
    }
    if (block*PAGE_SIZE >= size) {
      DEBUG(p("%d >= %d\n",block*PAGE_SIZE,size);)
      for (int j=first; j<=i; j++) {
        b = eeprom_read_byte((uint8_t *)PAGE_TABLE_OFFSET + j/4);
        DEBUG(p("b = %x\n",b);)
        if(j==i) {
          DEBUG(p("LAST\n");)
          entry = LAST;
        }
        else if (j == first) {
          DEBUG(p("FIRST\n");)
          entry = FIRST;
        }
        else {
          DEBUG(p("FILLED\n");)
          entry = FILLED;
        }
        DEBUG(p("writing %x to %d\n",b | (entry << ((j%4)*2)),(uint8_t *)PAGE_TABLE_OFFSET + j/4);)
        eeprom_write_byte((uint8_t *)PAGE_TABLE_OFFSET + j/4, b | (entry << ((j%4)*2)));
      }
      DEBUG(p("returning %x\n",PAGES_OFFSET + first*PAGE_SIZE));
      return (void*)(PAGES_OFFSET + first*PAGE_SIZE);
    }
  }
  return NULL;
}

void eepromFree(void* addr) {
  DEBUG(p("%s(%x)\n",__FUNCTION__,addr);)
  int page = ((unsigned int)addr-PAGES_OFFSET)/32;
  byte b;
  byte entry;
  do {
    b = eeprom_read_byte((uint8_t *)PAGE_TABLE_OFFSET + page/4);
    DEBUG(p("b = %x\n",b);)
    entry = (b >> ((page%4)*2))&0x03;
    DEBUG(p("entry = %x\n",entry);)
    if (entry == EMPTY) {
      Serial.print("ERROR freeing eeprom memory, not expecting empty page");
      return;
    }
    DEBUG(p("writing %x to %d\n",b & ~(0x3 << ((page%4)*2)),(uint8_t *)PAGE_TABLE_OFFSET + page/4);)
    eeprom_write_byte((uint8_t *)PAGE_TABLE_OFFSET + page/4, b & ~(0x3 << ((page%4)*2)));
    page += 1;
  } while (entry != LAST);
}

void eepromFreeAllPages() {
  DEBUG(p("%s()\n",__FUNCTION__);)
  for (int i=PAGE_TABLE_OFFSET; i<PAGE_TABLE_OFFSET+PAGE_TABLE_SIZE; i++) {
    eeprom_write_byte((uint8_t *)i, 0);
  }
}

unsigned long eepromGetId() {
  DEBUG(p("%s()\n",__FUNCTION__);)
  return eeprom_read_dword(ID_OFFSET);
}
#if 0
void eepromClear() {
  for (int i=0; i<EEPROM_SIZE; i++) {
    eeprom_write_byte((uint8_t*)i,0);
  }
}
#endif
