
void fault_action(void){
    int pos;
    auto call = id(awning).make_call();
    call.set_command_stop();
    call.perform();
    if (id(stall).state){
        // Window has stalled with an over current condition
        if (id(encoder).state < 0.25){
        // 1/4 rotation before closed, we must be at the closed endstop
        id(closed_to_endstop).publish_state(true);
        // Trigger an auto-recalibration to closed
        if (id(encoder).state < 0.125){
            // Close enough to mark at target
            id(at_target).publish_state(true);
        } else {
            // Need to recalibrate
            id(calibrate_closed).press();
        }
        } else if (id(encoder).state > id(max_position).state - 0.25){
        // 1/4 rotation before opened, we must be at the open endstop
        id(opened_to_endstop).publish_state(true);
        } else {
        // Not near the open/closed positions, flag problem
        id(window_stuck).publish_state(true);
        }             
    } else {
        // Overcurrent when the motor is runing, shouldnt happen
        id(current_limit).publish_state(true);
    }
    pos = round(100*id(encoder).state/id(max_position).state);
    if (pos <= 1) { pos = 0;}
    if (pos >=99) { pos = 100;}
    id(awning).position = pos;
    id(awning).publish_state();
}