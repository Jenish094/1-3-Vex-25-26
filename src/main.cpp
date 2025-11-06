#include "vex.h"
#include <list>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace vex;

//#region config_globals
vex::brain      Brain;
vex::motor      motor1(vex::PORT1, vex::gearSetting::ratio6_1, false);
vex::motor      motor10(vex::PORT10, vex::gearSetting::ratio6_1, true);
vex::controller con(vex::controllerType::primary);
//#endregion config_globals

////////////////////////////////////////////////////////////////////////////////
// Recording Code                                                             //
////////////////////////////////////////////////////////////////////////////////

// velocityRecordedMotor
//  Manages the recording, playback, saving, and loading of records of commanded
//  velocity.
//
// Usage:
//  Instantiation:
//      As vex::motor, but with a filename to refer to on SD card for permanent
//      recording.
//  As teleoperated drive motor:
//      As vex::motor
//  As recorded motor:
//      Call velocityRecordedMotor::spin with a target velocity. Internal logic 
//      will save the target velocity and timestamp.
//  As playback motor:
//      Call velocityRecordedMotor::spin, with or without arguments. Internal
//      logic will refer to a recording in memory and call motor::spin with
//      appropriate arguments.
//  To save/load:
//      Save - with a recording in memory, call saveRecording
//      Load - with a recording on SD card, call loadRecording
//
// Public methods:
//  All from vex::motor
//  Recording/playback controls. Changes behavior of spin method
//      enableRecording
//      disableRecording
//      enablePlayback
//      disablePlayback
//      disableRecordingOrPlayback
//  Save/load controls. SD card reading is expensive, don't use in match.
//      saveRecording
//      loadRecording
//  State information
//      isRecording
//      isPlayback
//      donePlayback
//      isIdle
//

class velocityRecordedMotor : public vex::motor {
    private:
    
    // This structure is used to store information on a single change of the
    // motor's velocity target. It encodes when the change was made, the
    // velocity (including direction), and what units the velocity was in
    struct dataslice {
        uint32_t timestamp;
        double normalizedVelocity;
        velocityUnits velocityType;
        
        dataslice() {
            
        }
        
        dataslice(uint32_t time, double vel, velocityUnits units) {
            timestamp = time;
            normalizedVelocity = vel;
            velocityType = units;
        }
    };
    
    // The data is stored in a doubly linked list
    std::list<dataslice> data;
    
    // This iterator is used to keep track of where in the recording the 
    // playback is currently at
    std::list<dataslice>::iterator playbackMarker;
    
    // This points to a string containing the save/load filename
    const char* logFileName;
    
    // These help keep track of timings. lastRecordingTime is used to make sure
    // that data isn't being saved too fast. operationBeginTime keeps track of
    // when playback or recording started to provide the proper offset.
    uint32_t lastRecordingTime;
    uint32_t operationBeginTime;
    
    // These help keep track of what the velocityRecordedMotor is doing and
    // should be doing.
    enum recordingState {none, recording, playback};
    recordingState myState = none;
    
    public:
    
    // The constructor for a velocityRecordedMotor. Delegates much of its task
    // to a constructor of the superclass, vex::motor.
    velocityRecordedMotor (int32_t index, vex::gearSetting gears, bool reversed, const char* logFileName)
      : vex::motor (index,gears,reversed) {
        this->logFileName = logFileName;
        lastRecordingTime = vex::timer::system();
    }
    
    void spin (vex::directionType dir, double velocity, vex::velocityUnits units) {
        uint32_t invocationTime = timer::system();
        if (myState == recording && invocationTime > lastRecordingTime) {
            vex::motor::spin(dir,velocity,units);
            if (dir == directionType::rev) velocity = -velocity;
            data.emplace_back(invocationTime - operationBeginTime,velocity,units);
            lastRecordingTime = invocationTime;
        } else if (myState == playback) {
            if (playbackMarker == data.end()) {
                stop(brakeType::hold);
            } else if (invocationTime >= operationBeginTime + playbackMarker->timestamp) {
                vex::motor::spin(vex::directionType::fwd, playbackMarker->normalizedVelocity, playbackMarker->velocityType);
                playbackMarker++;
            }
        } else if (myState == none) {
            vex::motor::spin(dir,velocity,units);
        }
    }
    
    void spin (directionType dir, double velocity, percentUnits units) {
        spin(dir,velocity,velocityUnits::pct);
    }
    
    void enableRecording() {
        lastRecordingTime = operationBeginTime = timer::system();
        data.clear();
        data.emplace_back(0,0,velocityUnits::pct);
        myState = recording;
    }
    
    bool isRecording() {
        return myState == recording;
    }
    
    bool isPlayback() {
        return myState == playback;
    }
    
    bool donePlayback() {
        return playbackMarker == data.end();
    }
    
    bool isIdle() {
        return myState == none;
    }
    
    void disableRecording() {
        if (myState == recording) {
            myState = none;
        }
    }
    
    void enablePlayback() {
        playbackMarker = data.begin();
        operationBeginTime = vex::timer::system();
        myState = playback;
    }
    
    void disablePlayback() {
        if (myState == playback) {
            myState = none;
        }
    }
    
    void disableRecordingOrPlayback () {
        myState = none;
    }
    
    void saveRecording() {
        disableRecording();
        std::ofstream outputFile(logFileName, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
        for (std::list<dataslice>::iterator it = data.begin(); it != data.end(); ++it) {
            outputFile.write((char*)&(it->timestamp),sizeof(dataslice::timestamp));
            outputFile.write((char*)&(it->normalizedVelocity),sizeof(dataslice::normalizedVelocity));
            outputFile.write((char*)&(it->velocityType),sizeof(dataslice::velocityType));
            outputFile.flush();
        }
        outputFile.close();
    }
    
    void loadRecording () {
        disableRecording();
        data.clear();
        std::ifstream inputFile(logFileName, std::ifstream::in | std::ifstream::binary);
        while (!inputFile.eof()) {
            dataslice nextInput;
            inputFile.read((char*)&(nextInput.timestamp),sizeof(dataslice::timestamp));
            inputFile.read((char*)&(nextInput.normalizedVelocity),sizeof(dataslice::normalizedVelocity));
            inputFile.read((char*)&(nextInput.velocityType),sizeof(dataslice::velocityType));
            data.push_back(nextInput);
        }
    }
    
    ~velocityRecordedMotor() {
        
    }
    // ...existing code...

    void exportToCppCode(const char* motorVariableName) {
        disableRecording();
        std::string cppFileName = std::string(logFileName) + ".cpp";
        std::ofstream outputFile(cppFileName.c_str(), std::ofstream::out | std::ofstream::trunc);
        outputFile << "// VexCode Recorder->Code - Jenish Pathak 2025 " << motorVariableName << std::endl;
        outputFile << "// Motor " << motorVariableName << std::endl;
        outputFile << std::endl;
        
        for (std::list<dataslice>::iterator it = data.begin(); it != data.end(); ++it) {
            if (it->normalizedVelocity != 0) {
                outputFile << motorVariableName << ".spin(";
                if (it->normalizedVelocity > 0) {
                    outputFile << "forward";
                } else {
                    outputFile << "reverse";
                }
                outputFile << ", " << abs(it->normalizedVelocity) << ", percent);" << std::endl;
            } else {
                outputFile << motorVariableName << ".stop();" << std::endl;
            }
            
            if (it != data.begin()) {
                auto prev = it;
                prev--;
                uint32_t waitTime = it->timestamp - prev->timestamp;
                if (waitTime > 0) {
                    outputFile << "wait(" << waitTime << ", msec);" << std::endl;
                }
            }
        }
        
        outputFile << motorVariableName << ".stop();" << std::endl;
        outputFile.close();
        
        // Also output to Brain screen for debugging
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Exported %d commands", data.size());
    }
    
    // Static method to export all motors together
    static void exportAllMotorsToVEXCode(velocityRecordedMotor* motors[], const char* motorNames[], int motorCount, const char* fileName) {
        std::ofstream outputFile(fileName, std::ofstream::out | std::ofstream::trunc);
        
        outputFile << "// Auto-generated VEXcode autonomous playback" << std::endl;
        outputFile << "// Copy this code into your autonomous function" << std::endl;
        outputFile << "void playRecording() {" << std::endl;
        
        // Find the maximum timestamp across all motors
        uint32_t maxTimestamp = 0;
        for (int i = 0; i < motorCount; i++) {
            if (!motors[i]->data.empty()) {
                uint32_t lastTime = motors[i]->data.back().timestamp;
                if (lastTime > maxTimestamp) {
                    maxTimestamp = lastTime;
                }
            }
        }
        
        // Generate time-based playback
        for (uint32_t currentTime = 0; currentTime <= maxTimestamp; currentTime += 20) {
            bool hasCommands = false;
            
            for (int i = 0; i < motorCount; i++) {
                for (auto it = motors[i]->data.begin(); it != motors[i]->data.end(); ++it) {
                    if (abs((int32_t)it->timestamp - (int32_t)currentTime) <= 10) {
                        if (!hasCommands) {
                            outputFile << "    // Time: " << currentTime << "ms" << std::endl;
                            hasCommands = true;
                        }
                        
                        if (it->normalizedVelocity != 0) {
                            outputFile << "    " << motorNames[i] << ".spin(";
                            if (it->normalizedVelocity > 0) {
                                outputFile << "forward";
                            } else {
                                outputFile << "reverse";
                            }
                            outputFile << ", " << abs(it->normalizedVelocity) << ", percent);" << std::endl;
                        } else {
                            outputFile << "    " << motorNames[i] << ".stop();" << std::endl;
                        }
                        break;
                    }
                }
            }
            
            if (hasCommands && currentTime < maxTimestamp) {
                outputFile << "    wait(20, msec);" << std::endl;
                outputFile << std::endl;
            }
        }
        
        // Stop all motors at the end
        outputFile << "    // Stop all motors" << std::endl;
        for (int i = 0; i < motorCount; i++) {
            outputFile << "    " << motorNames[i] << ".stop();" << std::endl;
        }
        
        outputFile << "}" << std::endl;
        outputFile.close();
        
        Brain.Screen.setCursor(1, 1);
        Brain.Screen.print("Exported combined code");
    }
};

////////////////////////////////////////////////////////////////////////////////
// End secret sauce                                                           //
////////////////////////////////////////////////////////////////////////////////


// DT motors
velocityRecordedMotor RecordedLF(vex::PORT1, vex::gearSetting::ratio6_1, false, "LFVlog.dat");
velocityRecordedMotor RecordedLB(vex::PORT2, vex::gearSetting::ratio6_1, false, "LBVlog.dat");
velocityRecordedMotor RecordedRF(vex::PORT3, vex::gearSetting::ratio6_1, false, "RFVlog.dat");
velocityRecordedMotor RecordedRB(vex::PORT4, vex::gearSetting::ratio6_1, false, "RBVlog.dat");

// OrbMover motors
velocityRecordedMotor recordedOrb5(vex::PORT5, vex::gearSetting::ratio18_1, false, "orb5Vlog.dat");
velocityRecordedMotor recordedOrb6(vex::PORT6, vex::gearSetting::ratio18_1, false, "orb6Vlog.dat");
velocityRecordedMotor recordedOrb7(vex::PORT7, vex::gearSetting::ratio18_1, false, "orb7Vlog.dat");

// helper wrappers to treat pairs/triples as motor groups for control and recording
static inline void left_mg_spin(double pct) {
    // left side reversed
    RecordedLF.spin(vex::directionType::fwd, -pct, vex::velocityUnits::pct);
    RecordedLB.spin(vex::directionType::fwd, -pct, vex::velocityUnits::pct);
}

static inline void right_mg_spin(double pct) {
    RecordedRF.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    RecordedRB.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
}

static inline void OrbMove_spin(double pct) {
    recordedOrb5.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    recordedOrb6.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
    recordedOrb7.spin(vex::directionType::fwd, pct, vex::velocityUnits::pct);
}


int main(void) {
    bool AlastPressed = false;
    bool BlastPressed = false;
    bool XlastPressed = false;
    bool YlastPressed = false;
    bool UplastPressed = false;
    bool checkForPlaybackEnd = false;
    while (true) {
        if (!AlastPressed && con.ButtonA.pressing()) {
            if (!RecordedLF.isRecording()) {
                // disable existing recordings
                RecordedLF.disableRecordingOrPlayback();
                RecordedLB.disableRecordingOrPlayback();
                RecordedRF.disableRecordingOrPlayback();
                RecordedRB.disableRecordingOrPlayback();
                recordedOrb5.disableRecordingOrPlayback();
                recordedOrb6.disableRecordingOrPlayback();
                recordedOrb7.disableRecordingOrPlayback();

                //enable recording
                RecordedLF.enableRecording();
                RecordedLB.enableRecording();
                RecordedRF.enableRecording();
                RecordedRB.enableRecording();
                recordedOrb5.enableRecording();
                recordedOrb6.enableRecording();
                recordedOrb7.enableRecording();

                con.Screen.setCursor(1,1);
                con.Screen.print("Recording...         ");
            } else {
                RecordedLF.disableRecordingOrPlayback();
                RecordedLB.disableRecordingOrPlayback();
                RecordedRF.disableRecordingOrPlayback();
                RecordedRB.disableRecordingOrPlayback();
                recordedOrb5.disableRecordingOrPlayback();
                recordedOrb6.disableRecordingOrPlayback();
                recordedOrb7.disableRecordingOrPlayback();
                con.Screen.clearLine(1);
            }
            AlastPressed = true;
        } else if (!BlastPressed && con.ButtonB.pressing()) {
            if (!RecordedLF.isPlayback()) {
                RecordedLF.disableRecordingOrPlayback();
                RecordedLB.disableRecordingOrPlayback();
                RecordedRF.disableRecordingOrPlayback();
                RecordedRB.disableRecordingOrPlayback();
                recordedOrb5.disableRecordingOrPlayback();
                recordedOrb6.disableRecordingOrPlayback();
                recordedOrb7.disableRecordingOrPlayback();

                RecordedLF.enablePlayback();
                RecordedLB.enablePlayback();
                RecordedRF.enablePlayback();
                RecordedRB.enablePlayback();
                recordedOrb5.enablePlayback();
                recordedOrb6.enablePlayback();
                recordedOrb7.enablePlayback();

                con.Screen.setCursor(1,1);
                con.Screen.print("Playback...        ");
                checkForPlaybackEnd = true;
            } else {
                RecordedLF.disableRecordingOrPlayback();
                RecordedLB.disableRecordingOrPlayback();
                RecordedRF.disableRecordingOrPlayback();
                RecordedRB.disableRecordingOrPlayback();
                recordedOrb5.disableRecordingOrPlayback();
                recordedOrb6.disableRecordingOrPlayback();
                recordedOrb7.disableRecordingOrPlayback();
                con.Screen.clearLine(1);
                checkForPlaybackEnd = false;
            }
            BlastPressed = true;
        } else if (!XlastPressed && con.ButtonX.pressing()) {
            RecordedLF.disableRecordingOrPlayback();
            RecordedLB.disableRecordingOrPlayback();
            RecordedRF.disableRecordingOrPlayback();
            RecordedRB.disableRecordingOrPlayback();
            recordedOrb5.disableRecordingOrPlayback();
            recordedOrb6.disableRecordingOrPlayback();
            recordedOrb7.disableRecordingOrPlayback();
            con.Screen.setCursor(1,1);
            uint32_t startLoadTime = timer::system();
            con.Screen.print("Saving...           ");
            RecordedLF.stop();
            RecordedLB.stop();
            RecordedRF.stop();
            RecordedRB.stop();
            recordedOrb5.stop();
            recordedOrb6.stop();
            recordedOrb7.stop();
            RecordedLF.saveRecording();
            RecordedLB.saveRecording();
            RecordedRF.saveRecording();
            RecordedRB.saveRecording();
            recordedOrb5.saveRecording();
            recordedOrb6.saveRecording();
            recordedOrb7.saveRecording();
            while (timer::system() < startLoadTime + 1000);
            con.Screen.clearLine(1);
            XlastPressed = true;
        } else if (!YlastPressed && con.ButtonY.pressing()) {
            RecordedLF.disableRecordingOrPlayback();
            RecordedLB.disableRecordingOrPlayback();
            RecordedRF.disableRecordingOrPlayback();
            RecordedRB.disableRecordingOrPlayback();
            recordedOrb5.disableRecordingOrPlayback();
            recordedOrb6.disableRecordingOrPlayback();
            recordedOrb7.disableRecordingOrPlayback();
            con.Screen.setCursor(1,1);
            uint32_t startLoadTime = timer::system();
            con.Screen.print("Loading...           ");
            RecordedLF.stop();
            RecordedLB.stop();
            RecordedRF.stop();
            RecordedRB.stop();
            recordedOrb5.stop();
            recordedOrb6.stop();
            recordedOrb7.stop();
            RecordedLF.loadRecording();
            RecordedLB.loadRecording();
            RecordedRF.loadRecording();
            RecordedRB.loadRecording();
            recordedOrb5.loadRecording();
            recordedOrb6.loadRecording();
            recordedOrb7.loadRecording();
            while (timer::system() < startLoadTime + 1000);
            con.Screen.clearLine(1);
            YlastPressed = true;
        } else if (!UplastPressed && con.ButtonUp.pressing()) {
            RecordedLF.disableRecordingOrPlayback();
            RecordedLB.disableRecordingOrPlayback();
            RecordedRF.disableRecordingOrPlayback();
            RecordedRB.disableRecordingOrPlayback();
            recordedOrb5.disableRecordingOrPlayback();
            recordedOrb6.disableRecordingOrPlayback();
            recordedOrb7.disableRecordingOrPlayback();
            con.Screen.setCursor(1,1);
            uint32_t startExportTime = timer::system();
            con.Screen.print("Exporting to VEXcode...");
            RecordedLF.stop();
            RecordedLB.stop();
            RecordedRF.stop();
            RecordedRB.stop();
            recordedOrb5.stop();
            recordedOrb6.stop();
            recordedOrb7.stop();
            
            // export all to same file
            velocityRecordedMotor* motors[] = {&RecordedLF, &RecordedLB, &RecordedRF, &RecordedRB, &recordedOrb5, &recordedOrb6, &recordedOrb7};
            const char* motorNames[] = {"RecordedLF", "RecordedLB", "RecordedRF", "RecordedRB", "recordedOrb5", "recordedOrb6", "recordedOrb7"};
            velocityRecordedMotor::exportAllMotorsToVEXCode(motors, motorNames, 7, "autonomous_playback.cpp");
            
            while (timer::system() < startExportTime + 1000);
            con.Screen.clearLine(1);
            UplastPressed = true;
        }
        AlastPressed = AlastPressed && con.ButtonA.pressing();
        BlastPressed = BlastPressed && con.ButtonB.pressing();
        XlastPressed = XlastPressed && con.ButtonX.pressing();
        YlastPressed = YlastPressed && con.ButtonY.pressing();
        UplastPressed = UplastPressed && con.ButtonUp.pressing();
        
        left_mg_spin(con.Axis3.position());
        right_mg_spin(con.Axis2.position());
        // L1 forward L2 Rev
        if (con.ButtonL1.pressing()) {
            OrbMove_spin(60);
        } else if (con.ButtonL2.pressing()) {
            OrbMove_spin(-60);
        } else {
            OrbMove_spin(0);
        }

        if (checkForPlaybackEnd && RecordedLF.donePlayback() && RecordedLB.donePlayback() && RecordedRF.donePlayback() && RecordedRB.donePlayback() && recordedOrb5.donePlayback() && recordedOrb6.donePlayback() && recordedOrb7.donePlayback()) {
            checkForPlaybackEnd = false;
            con.Screen.setCursor(1,1);
            con.Screen.print("Done playback    ");
            RecordedLF.disablePlayback();
            RecordedLB.disablePlayback();
            RecordedRF.disablePlayback();
            RecordedRB.disablePlayback();
            recordedOrb5.disablePlayback();
            recordedOrb6.disablePlayback();
            recordedOrb7.disablePlayback();
        }
        
        wait(30, msec);
    }
}
