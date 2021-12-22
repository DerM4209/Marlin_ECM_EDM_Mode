#include "../inc/MarlinConfig.h"
#include "../module/temperature.h"
#include "../gcode/gcode.h"
#include "../module/motion.h"
#include "../MarlinCore.h"
#include "../gcode/feature/ecm_edm_mode/G7_G8.h" //Because of compiling errors I included this file instead of doing it the opposite way

bool edm_ecm_setup = false;
bool target_reached = false;
bool reverse = false;
bool moving = false;
xyz_pos_t origin_position;
xyz_pos_t start_position;
xyz_pos_t target_position;

#ifdef ECM_EDM_MODE
void move_to_target(){
idle();
get_cartesian_from_steppers();
if (ecm_edm_voltage > TARGET_VOLTAGE && moving == false){ //at start when voltage is high and electrode is not moving, start moving
SERIAL_ECHOLNPGM("start moving towards target", ecm_edm_voltage);
report_current_position();
report_real_position();
moving = true;
destination = target_position;
prepare_line_to_destination();
 }
if (ecm_edm_voltage < TARGET_VOLTAGE && moving == true && reverse == false){ //when voltage is low and electrode is moving towards target
SERIAL_ECHOLNPGM("stop and move towards start", ecm_edm_voltage);
report_current_position();
report_real_position();
quickstop_stepper();
reverse = true;
destination = start_position;
prepare_line_to_destination();
}
if (ecm_edm_voltage > TARGET_VOLTAGE && moving == true && reverse == true){ //when voltage is high and electrode is moving towards start
SERIAL_ECHOLNPGM("stop and move towards target", ecm_edm_voltage);
report_current_position();
report_real_position();
quickstop_stepper();
reverse = false;
destination = target_position;
prepare_line_to_destination();
}
if (!planner.has_blocks_queued() && moving == true && reverse == false){
SERIAL_ECHOLNPGM("target reached");
start_position = target_position;
report_current_position();
report_real_position();
moving = false;
target_reached = true;
}
}
void ecm_edm_mode(){
if (run_ecm_edm_mode == true){
if (edm_ecm_setup == true){
origin_position = current_position;
start_position = current_position;
target_position = current_position;
new_destination = current_position;
edm_ecm_setup = false;
}
if (new_destination != target_position && new_destination != origin_position){
SERIAL_ECHOLNPGM("new target saved");
target_position = new_destination;
target_reached = false;
while (target_reached == false){
move_to_target();
}
}
}
else {
edm_ecm_setup = true;
}
}
  #endif