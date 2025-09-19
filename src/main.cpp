// VEX V5 C++ Project
#include "vex.h"
#include <list>
#include <algorithm>
#include <fstream>
#include <iostream>
using namespace vex;

//#region config_globals
vex::brain      Brain;
vex::motor      motor1(vex::PORT1, vex::gearSetting::ratio18_1, false);
vex::motor      motor10(vex::PORT10, vex::gearSetting::ratio18_1, true);
vex::controller con(vex::controllerType::primary);
//#endregion config_globals

////////////////////////////////////////////////////////////////////////////////
// Begin secret sauce                                                         //
////////////////////////////////////////////////////////////////////////////////

// velocityRecordedMotor
//  Manages the recording, playback, saving, and loading of records of commanded
//  velocity.
//
// Disclaimer: It is a violation of rule <G2> in VRC to copy my (John Tyler, an
//  adult) code verbatim. (Programs like this written by students are still
//  fine for competition use.)
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
};

////////////////////////////////////////////////////////////////////////////////
// End secret sauce                                                           //
////////////////////////////////////////////////////////////////////////////////


velocityRecordedMotor recordedMotor1(vex::PORT1, vex::gearSetting::ratio18_1, false, "motor1Vlog.dat");
velocityRecordedMotor recordedMotor10(vex::PORT10, vex::gearSetting::ratio18_1, true, "motor10Vlog.dat");


int main(void) {
    // Start here
    bool AlastPressed = false;
    bool BlastPressed = false;
    bool XlastPressed = false;
    bool YlastPressed = false;
    bool checkForPlaybackEnd = false;
    while (true) {
        if (!AlastPressed && con.ButtonA.pressing()) {
            if (!recordedMotor1.isRecording()) {
                recordedMotor1.disableRecordingOrPlayback();
                recordedMotor10.disableRecordingOrPlayback();
                recordedMotor1.enableRecording();
                recordedMotor10.enableRecording();
                con.Screen.setCursor(1,1);
                con.Screen.print("Recording...         ");
            } else {
                recordedMotor1.disableRecordingOrPlayback();
                recordedMotor10.disableRecordingOrPlayback();
                con.Screen.clearLine(1);
            }
            AlastPressed = true;
        } else if (!BlastPressed && con.ButtonB.pressing()) {
            if (!recordedMotor1.isPlayback()) {
                recordedMotor1.disableRecordingOrPlayback();
                recordedMotor10.disableRecordingOrPlayback();
                recordedMotor1.enablePlayback();
                recordedMotor10.enablePlayback();
                con.Screen.setCursor(1,1);
                con.Screen.print("Playback...        ");
                checkForPlaybackEnd = true;
            } else {
                recordedMotor1.disableRecordingOrPlayback();
                recordedMotor10.disableRecordingOrPlayback();
                con.Screen.clearLine(1);
                checkForPlaybackEnd = false;
            }
            BlastPressed = true;
        } else if (!XlastPressed && con.ButtonX.pressing()) {
            recordedMotor1.disableRecordingOrPlayback();
            recordedMotor10.disableRecordingOrPlayback();
            con.Screen.setCursor(1,1);
            uint32_t startLoadTime = timer::system();
            con.Screen.print("Saving...           ");
            recordedMotor1.stop();
            recordedMotor10.stop();
            recordedMotor1.saveRecording();
            recordedMotor10.saveRecording();
            while (timer::system() < startLoadTime + 1000);
            con.Screen.clearLine(1);
            XlastPressed = true;
        } else if (!YlastPressed && con.ButtonY.pressing()) {
            recordedMotor1.disableRecordingOrPlayback();
            recordedMotor10.disableRecordingOrPlayback();
            con.Screen.setCursor(1,1);
            uint32_t startLoadTime = timer::system();
            con.Screen.print("Loading...           ");
            recordedMotor1.stop();
            recordedMotor10.stop();
            recordedMotor1.loadRecording();
            recordedMotor10.loadRecording();
            while (timer::system() < startLoadTime + 1000);
            con.Screen.clearLine(1);
            YlastPressed = true;
        }
        AlastPressed = AlastPressed && con.ButtonA.pressing();
        BlastPressed = BlastPressed && con.ButtonB.pressing();
        XlastPressed = XlastPressed && con.ButtonX.pressing();
        YlastPressed = YlastPressed && con.ButtonY.pressing();
        
         recordedMotor1.spin(vex::directionType::fwd,con.Axis3.position(),vex::velocityUnits::pct);
        recordedMotor10.spin(vex::directionType::fwd,con.Axis2.position(),vex::velocityUnits::pct);
        
        if (checkForPlaybackEnd && recordedMotor1.donePlayback() && recordedMotor10.donePlayback()) {
            checkForPlaybackEnd = false;
            con.Screen.setCursor(1,1);
            con.Screen.print("Done playback    ");
            recordedMotor1.disablePlayback();
            recordedMotor10.disablePlayback();
        }
        
        wait(30, msec);
    }
}
