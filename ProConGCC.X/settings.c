/*
 * File:   settings.c
 * Author: Mitch
 *
 * Created on January 8, 2022, 3:31 PM
 */


#include <xc.h>
#include "main.h"

struct Settings SettingData = {
  .configKey = 0x0,
  .sx_low = 131,
  .sx_center = 254,
  .sx_high = 385,

  .sy_low = 140,
  .sy_center = 252,
  .sy_high = 398,

  .cx_low = 108,
  .cx_center = 252,
  .cx_high = 355,

  .cy_low = 127,
  .cy_center = 254,
  .cy_high = 385,

  .sx_highMultiplier = 200,
  .sx_lowMultiplier = 232,

  .sy_highMultiplier = 175,
  .sy_lowMultiplier = 228,

  .cx_highMultiplier = 257,
  .cx_lowMultiplier = 177,

  .cy_highMultiplier = 195,
  .cy_lowMultiplier = 201,
    
  .trigger_mode = 0x0,
  
};