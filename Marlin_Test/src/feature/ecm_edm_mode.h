#include "../inc/MarlinConfig.h"
#include "../gcode/gcode.h"

bool moving = false;
bool target_reached = true;
xyz_pos_t origin_position = current_position;
xyz_pos_t target_position = current_position;

#ifdef ECM_EDM_MODE
void ecm_edm_mode(){
if (new_destination != target_position && new_destination != origin_position && target_reached == true){
SERIAL_ECHOLNPGM("new target saved");
target_position = new_destination;
target_reached = false;
}

if (target_reached == false){
get_cartesian_from_steppers();

if (1000 > 500 && moving == false){
SERIAL_ECHOLNPGM("start moving towards target");
destination = target_position;
prepare_line_to_destination();
moving = true;
 }
 
if (NEAR(cartes, target_position)){
SERIAL_ECHOLNPGM("target reached");
target_reached = true;
moving = false;
}
}
}
  #endif
  
 