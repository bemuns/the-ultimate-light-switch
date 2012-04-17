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
#ifndef EEPROM_LAYOUT_H
#define EEPROM_LAYOUT_H

#include <Arduino.h>
#include <avr/EEPROM.h>

#define EEPROM_SIZE         1024
#define ID_OFFSET           0
#define ID_SIZE             4
#define TIME_SYNC_OFFSET    (ID_OFFSET + ID_SIZE)
#define TIME_SYNC_SIZE      4
#define PIN_STATE_OFFSET    (TIME_SYNC_OFFSET + TIME_SYNC_SIZE)
#define PIN_STATE_SIZE      3
#define PAGE_TABLE_OFFSET   (PIN_STATE_OFFSET + PIN_STATE_SIZE)
#define PAGE_SIZE           32
#define PAGE_TABLE_SIZE     ((EEPROM_SIZE - PAGE_TABLE_OFFSET)/PAGE_SIZE/4)
#define PAGES_OFFSET        (PAGE_TABLE_OFFSET + PAGE_TABLE_SIZE)
#define NUM_PAGES           ((EEPROM_SIZE - PAGES_OFFSET)/PAGE_SIZE)


void* eepromMalloc(int size);
void eepromFree(void* addr);
void eepromClear();
void eepromFreeAllPages();
unsigned long eepromGetId();
#endif
