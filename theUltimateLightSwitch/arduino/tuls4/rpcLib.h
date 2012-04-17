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
#ifndef RPC_LIB_H
#define RPC_LIB_H

#include "Arduino.h"
#include "jsonLib.h"

//Json * rpcEval(char*,Json *);
Json * rpcEval(Json * parent, Json * rpc);
void rpcDebug(char* type, int line);
//* = 0 or more
//+ = 1 or more
//? = 0 or 1
void p(char *fmt, ... );
#endif
