#include "config.h"
#include "motors/servo_control.h"
#include "Led/Led.h"
#include "Lcd/Lcd.h"
#include "Lcd/lcd_display_manager.h"
#include <Arduino_JSON.h>


void handleResponses(String topic, JSONVar payload);
void handleEntranceResponse(JSONVar response);
void handleExitResponse(JSONVar response);
void handleGateControl(JSONVar response);
void handleParkingSpaceResponse(JSONVar response);