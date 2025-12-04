#include "main.h"
#include "arms/config.h"

LV_IMG_DECLARE(logonat);
LV_IMG_DECLARE(logonatblue);
LV_IMG_DECLARE(logonatred);
// drivetrain set as chassis in ARMS/config.h
Controller master(CONTROLLER_MASTER);
	Motor rubber(1);
	Motor stage2(2);
	Motor topstage(3);
	Motor flywheel(4);
	Motor_Group intake ({rubber, stage2, topstage});
using namespace arms::chassis; // arms::chassis::function -> function
using namespace arms::selector; // arms::selector::function -> function

// set flywheel rpm using PID and BB
int threshold = 5;
float kF = 60;
float kP = 1;
void move_rpm(int target_speed) {
	float current_velocity = flywheel.get_actual_velocity();

	if(current_velocity < target_speed - threshold) {
		flywheel.move_voltage(12000);
	}
	else if(current_velocity > target_speed + threshold) {
		flywheel.move_voltage(0);
	}
	else {
		float p_component = (target_speed - current_velocity) * kP;
		float f_component = target_speed * kF;
		flywheel.move_voltage(p_component + f_component);
	}
}



void initialize() {
	arms::init();
}

void disabled() {
	master.rumble("- - -");
	master.print(1, 0, "Excuse the bad autonomous Alec");
	master.clear_line(1);
}

void competition_initialize() {

}

void autonomous() {
//  ____  _____ ____  
// |  _ \| ____|  _ \ 
// | |_) |  _| | | | |
// |  _ <| |___| |_| |
// |_| \_\_____|____/ 
	if(auton == 1) {
	//init image
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatred);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
		// for 34
		// turn right 90
		// forward 14
		// turn left -90
		// intake on
		// forward 10
		// forward 6
		// turn right 90
		// intake off
		// forward 14
		// turn right 90
		// forward 12
		// turn left 90
		// forward 17
		// right 90
		// reverse 6
		// outtake
		move(34);
		turn(90);
		move(14);
		turn(-90);
		{
			rubber = 127;
			stage2 = 127;

			move(10);
			move(6);
			rubber = 0;
			stage2 = 0;
	}
		turn(90);
		move(14);
		turn(90);
		move(12);
		turn(-90);
		move(17);
		turn(90);
		{
			Motor LG(12, 14);
			Motor RG(11, 13);
			Motor_Group drive({LG, RG});
			drive.move(-6);
			delay(10);
		}
		{
			intake = -127;

			delay(10000);

			intake = 0;
		}
	}
	else if(auton == 2) {
	//init image
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatred);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
		//RedRight
		move(34);
		turn(-90);
		move(14);
		turn(90);
		{
			rubber = 127;
			stage2 = 127;
			move(10);
			move(6);
			rubber = 0;
			stage2 = 0;
	}
		turn(-90);
		move(14);
		turn(-90);
		move(12);
		turn(90);
		move(17);
		turn(-90);
				{
			Motor LG(12, 14);
			Motor RG(11, 13);
			Motor_Group drive({LG, RG});
			drive.move(-6);
			delay(10);
		}
		{

			intake = -127;

			delay(10000);

			intake = 0;
		}
	}
	
	else if(auton == 3) {
		//Nothing will happen...
	}
//  ____  _    _   _ _____ 
// | __ )| |  | | | | ____|
// |  _ \| |  | | | |  _|  
// | |_) | |__| |_| | |___ 
// |____/|_____\___/|_____|

	else if(auton == -1) {
	//init image
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatblue);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
		//BlueLeft, redright copy
		move(34);
		turn(-90);
		move(14);
		turn(90);
		{
			rubber = 127;
			stage2 = 127;
			move(10);
			move(6);
			rubber = 0;
			stage2 = 0;
	}
		turn(-90);
		move(14);
		turn(-90);
		move(12);
		turn(90);
		move(17);
		turn(-90);
				{
			Motor LG(12, 14);
			Motor RG(11, 13);
			Motor_Group drive({LG, RG});
			drive.move(-6);
			delay(10);
		}
		{
			intake = -127;

			delay(10000);

			intake = 0;
		}
	}

	else if(auton == -2) {
	//init image
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatblue);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
		//BlueRight, redleft copy
		move(34);
		turn(90);
		move(14);
		turn(-90);
		{

			rubber = 127;
			stage2 = 127;

			move(10);
			move(6);
			rubber = 0;
			stage2 = 0;
	}
		turn(90);
		move(14);
		turn(90);
		move(12);
		turn(-90);
		move(17);
		turn(90);
				{
			Motor LG(12, 14);
			Motor RG(11, 13);
			Motor_Group drive({LG, RG});
			drive.move(-6);
			delay(10);
		}
		{

			intake = -127;

			delay(10000);

			intake = 0;
		}
	}
	
	else if(auton == -3) {
		//Nothing will happen...
	}

//  ____  _  _____ _     _     ____  
// / ___|| |/ /_ _| |   | |   / ___| 
// \___ \| ' / | || |   | |   \___ \ 
//  ___) | . \ | || |___| |___ ___) |
// |____/|_|\_\___|_____|_____|____/ 

	else if(auton == 0) {
		//init image
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonat);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
		
		// -90 deg turn (left)
		// 8 inch forward
		// 90 deg turn (right)
		// 17.13 forward
		// start rubber, stage 1
		// 5.43 forward
		// stop motors
		// -90 deg turn (left)
		// 8 inch forward
		// -90 deg turn (left)
		// 16.81 forward
		// 90 deg (right)
		// 21 forward
		// -90 deg (left)
		// 14 inch reverse
		// stage 1, stage 2, stage 3, flywheel and everything, reverse
		move(24);
		turn(-90);
		move(8);
		turn(90);
		move(17.13);
		{

			// start rubber and stage2 forward
			rubber = 127;
			stage2 = 127;

			// run drive while intake motors run
			move(5.43);

			// stop intake motors after move finishes
			rubber = 0;
			stage2 = 0;
		}
		turn(-90);
		move(8);
		turn(-90);
		move(16.81);
		turn(90);
		move(21);
		turn(-90);
		move(-14);
		{
			// reverse all intake motors
			intake = -127;

			// run for 10 second
			delay(10000);

			// stop all intake motors
			intake = 0;
		}
		
	}
	else {
		//..Once Again, do nothing
	}
}

void opcontrol() {
	//IMAGE
	if(auton == 1,2) {
	//init image red
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatred);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
	}
	else if(auton == -1,-2) {
	//init image blue
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonatblue);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
	}
	else {
	//init image white
	lv_obj_t *img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(img, &logonat);
	lv_obj_align(img, NULL, LV_ALIGN_CENTER, 0, 0);
	}
	master.rumble("- - -");
	master.print(1, 0, "alex cant drive");
	master.print(2, 0, "worlds this year frfr");


	while (true) {
		if (master.get_digital(DIGITAL_LEFT) && !competition::is_connected())
		    autonomous();
		arcade(master.get_analog(ANALOG_LEFT_Y) * (double)100 / 127,
			   master.get_analog(ANALOG_RIGHT_X) * (double)100 / 127);

		if (master.get_digital(DIGITAL_L1)) {
			intake = 127;
			move_rpm(600);
		}
		else if (master.get_digital(DIGITAL_L2)) {
			intake = -127;
			move_rpm(0);
		}
		else {
			intake = 0;
		}

		delay(20);
	}
	// logo

}