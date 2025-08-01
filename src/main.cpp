#include "vex.h"

using namespace vex;

brain Brain;
controller Controller1;

motor LeftFront(PORT1, ratio6_1, false);
motor BackLeft(PORT2, ratio6_1, false);
motor RightFront(PORT3, ratio6_1, true);
motor BackRight(PORT4, ratio6_1, true);
motor Chain(PORT5, ratio6_1, false);
motor IntakeTop(PORT6, ratio6_1, false);
motor IntakeBottom(PORT7, ratio6_1, true); // reversed

int main() {
    Brain.Screen.print("USB Logging Started");

    while (true) {
        int leftPower = Controller1.Axis3.position();
        int rightPower = Controller1.Axis2.position();

        LeftFront.spin(forward, leftPower, percent);
        BackLeft.spin(forward, leftPower, percent);
        RightFront.spin(forward, rightPower, percent);
        BackRight.spin(forward, rightPower, percent);

        if (Controller1.ButtonR1.pressing()) {
            IntakeTop.spin(forward, 100, percent);
            IntakeBottom.spin(forward, 100, percent);
        } else if (Controller1.ButtonR2.pressing()) {
            IntakeTop.spin(reverse, 100, percent);
            IntakeBottom.spin(reverse, 100, percent);
        } else {
            IntakeTop.stop();
            IntakeBottom.stop();
        }

        if (Controller1.ButtonL1.pressing()) {
            Chain.spin(forward, 100, percent);
        } else if (Controller1.ButtonL2.pressing()) {
            Chain.spin(reverse, 100, percent);
        } else {
            Chain.stop();
        }

        printf("Left: %d, Right: %d | R1: %d R2: %d | L1: %d L2: %d\n",
               leftPower,
               rightPower,
               Controller1.ButtonR1.pressing(),
               Controller1.ButtonR2.pressing(),
               Controller1.ButtonL1.pressing(),
               Controller1.ButtonL2.pressing());

        this_thread::sleep_for(100);
    }
}
