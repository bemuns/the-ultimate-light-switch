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
#ifndef TULS_H
#define TULS_H

#include "Arduino.h"
#include "byteStream.h"

#define MAGIC_START 210
#define MAGIC_STOP 222

void p(char *fmt, ... );

inline int seconds() {
  return (int)((long)millis())/1000;
}

#endif
