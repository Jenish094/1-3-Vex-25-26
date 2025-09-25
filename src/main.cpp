#include "main.h"
#include "pros-grafana-lib/api.h"


void initialize() {

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
Runs After initialize() but before autonomous while cdonnected to the field
 */
void competition_initialize() {

}

void autonomous() {

}

void opcontrol() {
	Controller master(E_CONTROLLER_MASTER);

	// init grafana
	grafanalib::GUIManager manager;
	manager.setRefreshRate(20);
	// MOTORS
	Motor LF(1);
	Motor RF(2);
	Motor LB(3);
	Motor RB(4);

	grafanalib::Variable<Motor> leftFrontMotorVar("Left Front Motor", LF);
	grafanalib::Variable<Motor> rightFrontMotorVar("Right Front Motor", RF);
	grafanalib::Variable<Motor> leftBackMotorVar("Left Back Motor", LB);
	grafanalib::Variable<Motor> rightBackMotorVar("Right Back Motor", RB);
	grafanalib::VariableGroup<Motor> chassisVars({leftFrontMotorVar, rightFrontMotorVar, leftBackMotorVar, rightBackMotorVar});
	chassisVars.add_getter("Temperature", &Motor::get_temperature);
	chassisVars.add_getter("Actual Velocity", &Motor::get_actual_velocity);
	chassisVars.add_getter("Voltage", &Motor::get_voltage);
	chassisVars.add_getter("Efficiency", &Motor::get_efficiency);
	manager.registerDataHandler(&chassisVars);
	manager.startTask();
	while (true) {
		delay(20);
	}

}
