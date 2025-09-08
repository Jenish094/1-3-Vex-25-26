#include "main.h"
#include "pros-grafana-lib/api.h"

#define LeftFront 10
#define LeftBack 9
#define RightFront 20
#define RightBack 11
#define Green 8
#define Blue 5
#define Orange 15
#define Yellow 6
#define Purple 7
lv_obj_t* image;
// 480x247 is screen res
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */


int autonSelection = 0;
	// 0 none
	// 1RL
	// 2RR
	// 3BL
	// 4BR
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
static void btn_event_cb(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    if (code != LV_EVENT_CLICKED) return;

    lv_obj_t *btn = lv_event_get_target(e);

    if (btn == btnLRed)       autonSelection = 1;
    else if (btn == btnRRed)  autonSelection = 2;
    else if (btn == btnLBlue) autonSelection = 3;
    else if (btn == btnRBlue) autonSelection = 4;

    pros::lcd::print(0, "Auton Selected: %d", autonSelection);
}


void initialize() {
	const lv_coord_t w = 240;
	const lv_coord_t h = 120;
   // Red L
    btnLRed = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btnLRed, w, h);
    lv_obj_align(btnLRed, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(btnLRed, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_add_event_cb(btnLRed, btn_event_cb, LV_EVENT_CLICKED, NULL);
    {
      lv_obj_t *lbl = lv_label_create(btnLRed);
      lv_label_set_text(lbl, "L");
      lv_obj_center(lbl);
    }

  // Red R
    btnRRed = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btnRRed, w, h);
    lv_obj_align(btnRRed, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_set_style_bg_color(btnRRed, lv_color_hex(0xFF0000), LV_PART_MAIN);
    lv_obj_add_event_cb(btnRRed, btn_event_cb, LV_EVENT_CLICKED, NULL);
    {
      lv_obj_t *lbl = lv_label_create(btnRRed);
      lv_label_set_text(lbl, "R");
      lv_obj_center(lbl);
    }

  //Blue L
    btnLBlue = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btnLBlue, w, h);
    lv_obj_align(btnLBlue, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_style_bg_color(btnLBlue, lv_color_hex(0x0000FF), LV_PART_MAIN);
    lv_obj_add_event_cb(btnLBlue, btn_event_cb, LV_EVENT_CLICKED, NULL);
    {
      lv_obj_t *lbl = lv_label_create(btnLBlue);
      lv_label_set_text(lbl, "L");
      lv_obj_center(lbl);
    }

  // Blue R
    btnRBlue = lv_btn_create(lv_scr_act());
    lv_obj_set_size(btnRBlue, w, h);
    lv_obj_align(btnRBlue, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_bg_color(btnRBlue, lv_color_hex(0x0000FF), LV_PART_MAIN);
    lv_obj_add_event_cb(btnRBlue, btn_event_cb, LV_EVENT_CLICKED, NULL);
    {
      lv_obj_t *lbl = lv_label_create(btnRBlue);
      lv_label_set_text(lbl, "R");
      lv_obj_center(lbl);
    }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}
// void autontest() {
// 	MotorGroup Intake({-Green, Blue, Orange, -Yellow, Purple});
// 	Intake.move(kIntakePower);
// }
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

void autonomous() {
    switch (autonSelection) {
        case 1: 
			autLRed();
			pros::lcd::print(0, "Red Left");
		break;
        case 2: 
			autRRed(); 
			pros::lcd::print(0, "Red Right");
			break;
        case 3: 
			autLBlue(); 
			pros::lcd::print(0, "Blue Left");
			break;
        case 4: 
			autRBlue(); 
			pros::lcd::print(0, "Blue Right");
			break;
        default:
            pros::lcd::print(0, "No auton selected!");
            break;
    }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void autonRL() {

}
void autonRR(){

}
void autonBL(){

}
void autonBR(){

}
void opcontrol() {
  
	LV_IMG_DECLARE(Image);
	image = lv_img_create(lv_scr_act());
	lv_img_set_src(image, &Image);
	lv_obj_set_size(image, 480, 272);
	lv_obj_align(image, LV_ALIGN_CENTER, 0, 0);

  // PROS Grafana
  auto manager = std::make_shared<grafanalib::GUIManager>();
  manager.setRefreshRate(20);
  grafanalib::Variable<pros::Motor> leftFrontMotorVar("Left Front Motor", LeftFront);
  grafanalib::Variable<pros::Motor> leftBackMotorVar("Left Back Motor", LeftBack);
  grafanalib::Variable<pros::Motor> rightFrontMotorVar("Right Front Motor", RightFront);
  grafanalib::Variable<pros::Motor> rightBackMotorVar("Right Back Motor", RightBack);
  chassisVars.add_getter("Temperature", &pros::Motor::get_temperature);
	chassisVars.add_getter("Actual Velocity", &pros::Motor::get_actual_velocity);
	chassisVars.add_getter("Voltage", &pros::Motor::get_voltage);
	chassisVars.add_getter("Efficiency", &pros::Motor::get_efficiency);
	manager.registerDataHandler(&chassisVars);
	manager.startTask();
  // End Pros Grafana Run
	Controller master(E_CONTROLLER_MASTER);
	lcd::set_text(1, "Running Driver");

  MotorGroup left_mg ({LeftFront, LeftBack});	
	MotorGroup right_mg ({RightFront, RightBack});
	// Intake orientation mapping per requirements (on forward command):
	// Green reverse, Blue forward, Orange forward, Yellow reverse, Purple forward
	// Use negative port numbers to reverse those motors in the group.
	MotorGroup Intake({-Green, Blue, Orange, -Yellow, Purple});


	while (true) {
		lcd::print(0, "%d %d %d", (lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		                 (lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		                 (lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);  // Prints status of the emulated screen LCDs


		// Arcade control scheme
		int dir = master.get_analog(ANALOG_LEFT_Y);    // Gets amount forward/backward from left joystick
		int turn = master.get_analog(ANALOG_RIGHT_X);  // Gets the turn left/right from right joystick
		left_mg.move(dir - turn);                      // Sets left motor voltage
		right_mg.move(dir + turn);                     // Sets right motor voltage

		// Intake control: R1 forward, R2 reverse (others stop)
		if (master.get_digital(E_CONTROLLER_DIGITAL_R1)) {
			Intake.move(127);
		} else if (master.get_digital(E_CONTROLLER_DIGITAL_R2)) {
			Intake.move(-127);
		} else {
			Intake.move(0);
		}
		delay(20);                               // Run for 20 ms then update
	}
}