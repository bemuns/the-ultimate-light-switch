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
