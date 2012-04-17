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

//#include <EEPROM.h>
#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/EEPROM.h>

static const char *inputStr,*inputStrEnd;
static char *outputStr;

byte serialReadByte() {
  while(Serial.available() <= 0);
  return Serial.read();
}

byte serialPeekByte() {
  while(Serial.available() <= 0);
  return Serial.peek();
}

void serialWriteByte(byte b) {
  Serial.write(b);
}

void setInputStr(const char* input, int size) {
  inputStr = input;
  inputStrEnd = input+size;
}
void setOutputStr(char * output) {
  outputStr = output;
}

bool checkInputStr() {
  if (inputStr >= inputStrEnd) {
      Serial.println("ERROR: parsing went beyond end of input string\n");
      return false;
  }
  return true;
}

byte ramReadByte(void) {
  if(!checkInputStr()) return 0;
  return *(inputStr++);
}

byte ramPeekByte(void) {
  if(!checkInputStr()) return 0;
  return *(inputStr);
}

byte ramWriteByte(byte b) {
  *(outputStr++) = b;
}

byte flashReadByte(void) {
  if(!checkInputStr()) return 0;
  return pgm_read_byte(inputStr++);
}

byte flashPeekByte(void) {
  if(!checkInputStr()) return 0;
  return pgm_read_byte(inputStr);
}

byte eepromReadByte(void) {
  if(!checkInputStr()) return 0;
  return eeprom_read_byte((const uint8_t *)inputStr++);
}

byte eepromPeekByte(void) {
  if(!checkInputStr()) return 0;
  return eeprom_read_byte((const uint8_t *)inputStr);
}

void eepromWriteByte(byte b) {
  eeprom_busy_wait();
  eeprom_write_byte((uint8_t*)outputStr++,(uint8_t)b);
}
