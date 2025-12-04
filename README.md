# 1-3-Vex-25-26
## Code for robot for the 2025/2026 season for 1/3

### As of 4/12/2025
### nationals25
- Code for Nationals Competition 2025
- Includes Skills, Autonomous Selector (using LVGL), PID (used for the flywheel)


Notes:

- Convert to PROS 3 rather than PROS 4 due to more documentation for 3, better memory management and suport for PROS-grafana to monitor telementry easily with a GUI on my laptop and better support for the Motor Recording autonomous
- Adda thing so that the autonomous position can be selected (Right or Left) by the controller rather than the horrible brain interaction which just straight up doesnt work. Only need 2 autonomous code as BL and RR, BR and RL are the exact same. Set this in void initialize() so it can be chosen before the match starts while plugged into the field
