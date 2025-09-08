#include "main.h"

#define LeftFront 10
#define LeftBack 9
#define RightFront 20
#define RightBack 11
#define Green 8
#define Blue 5
#define Orange 15
#define Yellow 6
#define Purple 7

// 480x247 is screen res

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */

void initialize() {

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

void opcontrol() {
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