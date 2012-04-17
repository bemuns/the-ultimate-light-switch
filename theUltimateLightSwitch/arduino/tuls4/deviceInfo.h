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
#ifndef DEVICE_INFO_H
#define DEVICE_INFO_H
#include "jsonLib.h"
#include <avr/pgmspace.h>
PROGMEM prog_char deviceLocationStr[]="{\"area\":\"W\",\"room\":\"Kids Bath\",\"floor\":2}";
PROGMEM prog_char deviceSensorsStr[]="{\"Current\":{\"pin\":3},\"Motion\":{\"pin\":4},\"Temp\":{\"pin\":2},\"Humidity\":{\"pin\":1}}";
PROGMEM prog_char deviceActuatorsStr[]="{\"Lights\":{\"type\":\"relay\",\"pin\":5},\"Fan\":{\"type\":\"relay\",\"pin\":6}}";
PROGMEM prog_char deviceRpcStr[]="{\"add\":{\"params\":[\"int\",\"int\"],\"result\":\"int\"},\"alarm\":{\"params\":[\"time\",\"script\",{\"repeat\":\"int?\",\"period\":\"time?\",\"label\":\"str?\"}],\"result\":\"bool\"},\"getId\":{\"params\":[],\"result\":\"int\"}}";
#endif
