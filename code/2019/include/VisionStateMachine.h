/*
 * VisionStateMachine.h
 *
 *  Created on: Jan 20, 2019
 *      Author: AnishaKabir
 */
#include "Settings.h"
#include "frc/WPILib.h"
#include "jankyStateMachine.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"


#define SRC_VISIONSTATEMACHINE_H_

class VisionStateMachine: public JankyStateMachine {
public:
    #ifdef JANKY_BOT_2019
    VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*flmotor, WPI_TalonSRX*frmotor, WPI_VictorSPX*rlmotor, WPI_VictorSPX*rrmotor);
    #else
    VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*flmotor, WPI_TalonSRX*frmotor, WPI_TalonSRX*rlmotor, WPI_TalonSRX*rrmotor);
    #endif
    virtual ~VisionStateMachine();

    enum StateValue {
        Idle,
        AutoDrive,
        DriveComplete,
        ScoreCargoOnCargoShip,
        ScoreCargoOnRocketShip, //assuming that scoring cargo on cargoship and scoring on rocket ship are a bit different
        ScoreHatchPanel,
        CancelActions,
        DriveSegment,
        TurnSegment
    };

    void StateEngine(int curState);
    void StartSequence();
    void StartSequenceTest();
    bool IsIdle();
    void Cancel();

    bool isIdle;
    bool isCancelled;
    bool testMode;
    bool regMode;
};