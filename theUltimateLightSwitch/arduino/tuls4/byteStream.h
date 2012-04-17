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
#ifndef BYTE_STREAM_H
#define BYTE_STREAM_H
#include <Arduino.h>
#include <avr/pgmspace.h>

byte serialReadByte();

byte serialPeekByte();

void serialWriteByte(byte);

void setInputStr(const char* start, int size);

bool checkInputStr();

byte ramReadByte(void);

byte ramPeekByte(void);

byte flashReadByte(void);

byte flashPeekByte(void);


#endif
