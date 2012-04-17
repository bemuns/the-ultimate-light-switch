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
//This file is generated automatically, do not edit
#ifndef RPC_INTERFACE_H
#define RPC_INTERFACE_H
#include "jsonLib.h"
void pinMode(int, int);
void digitalWrite(int, int);
const unsigned int digitalRead(int);
unsigned int analogRead(int);
void analogReference(int);
void analogWrite( unsigned int , int);
#endif
