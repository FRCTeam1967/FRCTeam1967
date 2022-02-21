
package org.janksters.jankyLib;
//import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

public class JankyTalonFX extends WPI_TalonFX {
    private final int kTimeoutMs = 1;
    private final int kPIDLoopIdx = 0;

    public JankyTalonFX(int channelNum) {
        super(channelNum);
    }

    public void setUpJankyTalonFX(){
        configFactoryDefault();
        configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, kPIDLoopIdx, kTimeoutMs);
        setSensorPhase(true);
        setInverted(false);

        configNominalOutputForward(0, kTimeoutMs);
        configNominalOutputReverse(0, kTimeoutMs);
        configPeakOutputForward(1, kTimeoutMs);
		configPeakOutputReverse(-1, kTimeoutMs);
    }
}
