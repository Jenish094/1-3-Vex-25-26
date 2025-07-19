/*---------------------------------------------------------------------------*/
/*                          Defining Robot Devices                           */
/*---------------------------------------------------------------------------*/

brain Brain;
controller Controller1;

// Here just change the port number to match the actual robot
// Drivetrain
motor FrontLmotor(PORT2, ratio6_1, false); //front left motor
motor BackLmotor(PORT1, ratio6_1, false); // back left motor
motor FrontRmotor(PORT3, ratio6_1, true); // reversed- front right motor
motor BackRmotor(PORT4, ratio6_1, true);  // reversed- back right motor
// 6:1 ratio was used to accellerate speed as our preseason theories lead to a fast-paced game

// Other devices
motor IntakeMotor(PORT5, ratio6_1, true); // intake motor
motor IntakeMotor2(PORT6, ratio6_1, false); // second intake motor
motor_group IntakeWheels(IntakeMotor, IntakeMotor2 ); // intake wheels motor group

motor Chain(PORT7, ratio6_1, false); //Chain
