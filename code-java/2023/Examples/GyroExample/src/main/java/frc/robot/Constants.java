package frc.robot;

import com.ctre.phoenix.sensors.Pigeon2.AxisDirection;

public final class Constants {
    public final static class Gyroscope {
        public final static int canID = 1;
        public final static AxisDirection forwardAxis = AxisDirection.PositiveY;
        public final static AxisDirection upAxis = AxisDirection.PositiveZ;
    }

    public final static class LED {
        public final static int pwmPin = 0;
        public final static int width = 48;
        public final static int height = 16;
    }
}
