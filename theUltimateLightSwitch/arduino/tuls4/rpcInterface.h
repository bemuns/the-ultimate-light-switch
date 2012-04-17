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
//This file is generated automatically, do not edit
#ifndef RPC_INTERFACE_H
#define RPC_INTERFACE_H
#include "jsonLib.h"
Json * rpcFunc(char * method, Json * params);
Json* rpc_add(Json*);
Json* rpc_alarm(Json*);
Json* rpc_getId(Json*);
#endif
