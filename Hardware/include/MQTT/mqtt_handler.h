#include "config.h"
#include "motors/servo_control.h"
#include <Arduino_JSON.h>


void handleResponses(String topic, JSONVar payload);
void handleEntranceResponse(JSONVar response);
void handleExitResponse(JSONVar response);
void handleGateControl(JSONVar response);