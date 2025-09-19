#include "main.h"
#include "liblvgl/display/lv_display.h"
#include "liblvgl/widgets/image/lv_image.h"

#define LeftFront 10
#define LeftBack 9
#define RightFront 20
#define RightBack 11
#define Green 8
#define Pink 5
#define BluePurple 15
#define Orange 6
#define Yellow 7


void displayimg() {
 LV_IMAGE_DECLARE(image);
 lv_obj_t* img = lv_image_create(lv_screen_active());
 lv_image_set_src(img, &image);
 lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
}



int press_x, press_y;

void initialize() {

 pros::screen::set_pen(0xff0000);
 pros::screen::fill_rect(0, 0, 238, 120);
 pros::screen::fill_rect(0, 124, 238, 240);
 pros::screen::set_pen(0x0000ff);
 pros::screen::fill_rect(242, 0, 480, 120);
 pros::screen::fill_rect(242, 124, 480, 240);
 while (1) {
   if (pros::screen::touch_status().touch_status == E_TOUCH_PRESSED) {
       press_x = pros::screen::touch_status().x;
       press_y = pros::screen::touch_status().y;
       break;
   }
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
void autonRL() {
    while(1){
       pros::screen::print(pros::E_TEXT_MEDIUM, 3, "RL");
       pros::delay(1000);
    }
  }
void autonRR(){
    while(1){
       pros::screen::print(pros::E_TEXT_MEDIUM, 3, "RR");
       pros::delay(1000);
    }
}
void autonBL(){
    while(1){
       pros::screen::print(pros::E_TEXT_MEDIUM, 3, "BL");
       pros::delay(1000);
    }
}
void autonBR(){
    while(1){
       pros::screen::print(pros::E_TEXT_MEDIUM, 3, "BR");
       pros::delay(1000);
    }
}
void autonomous() {
  if ((press_x < 240) && (press_y < 120)) { 
    autonRL();
  } else if ((press_x >= 241) && (press_y < 120)) {
    autonRR();
  } else if ((press_x > 240) && (press_y >= 121)) {
    autonBL();
  } else if ((press_x >= 241) && (press_y >= 121)) {
    autonBR();
  }
 delay (15);

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

  MotorGroup left_mg ({LeftFront, LeftBack});	
	MotorGroup right_mg ({RightFront, RightBack});
	// Intake orientation mapping per requirements (on forward command):
	// Use negative port numbers to reverse those motors in the group.
	MotorGroup Intake({-Green, Pink, BluePurple, -Yellow});


	while (true) {

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
	delay (15);
  displayimg();

}