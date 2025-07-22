#include "drive.h"
#include "devices.h"
#include "vex.h"

using namespace vex;

void drive(double distanceInInches, double speedPercent) {
  FrontLmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  BackLmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  FrontRmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  BackRmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct);
}

void turn(double degrees, double speedPercent) {
  const double WB = 12.0; // change this to whatever the distance between the 2 front weels are
  double turnCircumference = WB * M_PI;
  double distanceInInches = (degrees / 360.0) * turnCircumference;
  
  // Left side reverse, right side forward
  FrontLmotor.spinFor(reverse, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  BackLmotor.spinFor(reverse, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  FrontRmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct, false);
  BackRmotor.spinFor(forward, distanceInInches/12.56, turns, speedPercent, velocityUnits::pct);
}
