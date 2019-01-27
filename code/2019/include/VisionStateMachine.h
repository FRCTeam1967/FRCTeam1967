/*
 * VisionStateMachine.h
 *
 *  Created on: Jan 20, 2019
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#include "jankyStateMachine.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"


#define SRC_VISIONSTATEMACHINE_H_

class VisionStateMachine: public JankyStateMachine {
public:
    VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor);
    virtual ~VisionStateMachine();

    enum StateValue {
        Idle,
        AutoDrive,
        DriveComplete,
        ScoreCargoOnCargoShip,
        ScoreCargoOnRocketShip, //assuming that scoring cargo on cargoship and scoring on rocket ship are a bit different
        ScoreHatchPanel,
        CancelActions
    };

    void StateEngine(int curState);
    void StartSequence();
    void Cancel();

    bool isIdle;
    bool isCancelled;
    bool testMode;
    bool regMode;
};