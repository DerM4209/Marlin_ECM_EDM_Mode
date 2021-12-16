#include "../inc/MarlinConfig.h"
#include "../module/temperature.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"

bool edm_ecm_setup = true;
bool target_reached = false;
bool reverse = false;
xyz_pos_t origin_position;
xyz_pos_t start_position;
xyz_pos_t target_position;

#ifdef ECM_EDM_MODE

void move_to_target(){
idle();
if (ecm_edm_voltage > TARGET_VOLTAGE && reverse == false){
destination = target_position;
prepare_line_to_destination();
SERIAL_ECHOLNPGM("towards target", ecm_edm_voltage);
 }
if (ecm_edm_voltage < TARGET_VOLTAGE && reverse == false){
planner.quick_stop();
planner.synchronize();
reverse = true;
destination = start_position;
prepare_line_to_destination();
SERIAL_ECHOLNPGM("towards start", ecm_edm_voltage);
}
if (ecm_edm_voltage > TARGET_VOLTAGE && reverse == true){
planner.quick_stop();
planner.synchronize();
reverse = false;
}
}

void ecm_edm_mode(){
if (edm_ecm_setup == true){
origin_position = current_position;
start_position = current_position;
target_position = current_position;
edm_ecm_setup = false;
}
if (new_destination != target_position && new_destination != origin_position){
target_position = new_destination;
target_reached = false;
SERIAL_ECHOLNPGM("new target saved!");
while (target_reached == false){
move_to_target();
}
}
}
  #endif
  
/*
if (ecm_edm_voltage > GAP_LOWER_LIMIT && ecm_edm_voltage < GAP_UPPER_LIMIT){
quickstop_stepper();
}
}
//const millis_t ms = millis();
//static millis_t message_event_ms = 0;
 //if (ELAPSED(ms, message_event_ms)){
//message_event_ms = ms + 2000;
//SERIAL_ECHO_MSG(STR_BUSY_PROCESSING);
//}
 /*
do_blocking_move_to(target_position, feedrate_mm_s);
if (ecm_edm_voltage < GAP_UPPER_LIMIT && ecm_edm_voltage > GAP_LOWER_LIMIT){
quickstop_stepper();
}
}
}
/*if (ecm_edm_voltage > GAP_LOWER_LIMIT && ecm_edm_voltage < GAP_UPPER_LIMIT){
SERIAL_ECHOLNPGM("Gap OK!", ecm_edm_voltage);
destination = current_position;
prepare_line_to_destination();
if (destination != current_position){
quickstop_stepper();
}
}
/*if (current_position == target_position){
SERIAL_ECHOLNPGM("target reached!");	
const millis_t wait = millis() + 500;
while (PENDING(millis(), wait)) idle();
start_position = target_position;
SERIAL_ECHOLNPGM("position secured, continue!");
}*/