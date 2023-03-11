package frc.robot;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.util.Color;

public class LED {
    public int m_width = 1;
    public int m_height = 1;
    public double m_brightness = 0.25;
    
    private AddressableLED m_led;
    private AddressableLEDBuffer m_ledBuffer;
    
    private int ledStatus = 3;

    //sequence fields 
    private Timer seqTimer = new Timer();
    private Color seqColorOne, seqColorTwo;

    //flashing
    private Color currentSeqColor;
    private double flashInterval;

    //chasing
    private int chaseSegmentLength;
    private int chaseStartPixIdx = 0; //where in the RRBB is my starting pixel
    private double chaseInterval;

    //wakeup
    private Timer wakeupTimer = new Timer();

    ShuffleboardTab ledTab;

    public LED(int width, int height, int pwmPin) {
        m_width = width > 0 ? width : 1;
        m_height = height > 0 ? height : 1;

        //setup the buffer
        m_ledBuffer = new AddressableLEDBuffer(m_height * m_width);
        setColor(0, 0, 0);

        //setup the LED updating system
        m_led = new AddressableLED(pwmPin);
        m_led.setLength(m_ledBuffer.getLength());
        m_led.setData(m_ledBuffer);
        m_led.start();

        ledTab = Shuffleboard.getTab("LED");
    }

    public void setPixelByID(int pixelID, int red, int green, int blue) {
        int scaledRed = (int)((double)red * m_brightness);
        int scaledGreen = (int)((double)green * m_brightness);
        int scaledBlue = (int)((double)blue * m_brightness);

        m_ledBuffer.setRGB(pixelID, (int)scaledRed, (int)scaledGreen, (int)scaledBlue);
    }

    public void setPixelByID(int pixelID, Color color) {
        setPixelByID(pixelID, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
    }

    //set all pixels to the same color- rbg values
    public void setColor(int red, int green, int blue) {
        for (int i = 0; i < m_width * m_height; i++) {
            setPixelByID(i, red, green, blue);
        }
    }

    //set all pixels to the same color- standard color
    public void setColor(Color color) {
        for (int i = 0; i < m_width * m_height; i++) {
            setPixelByID(i, (int)(color.red * 255), (int)(color.green * 255), (int)(color.blue * 255));
        }
    }

    //sets up color flashing variables
    public void setFlashColors(Color colorOne, Color colorTwo, double interval){
        ledStatus = 0; //indicates- status is flashing

        seqColorOne = colorOne;
        seqColorTwo = colorTwo;
        flashInterval = interval;
        
        setColor(colorOne);
        currentSeqColor = colorOne;
        seqTimer.start();
        commit();
    }

    public void setChasingColors(Color colorOne, Color colorTwo, int segmentLength, double interval){
        ledStatus = 1; //indicates- status is chasing

        seqColorOne = colorOne;
        seqColorTwo = colorTwo;
        chaseInterval = interval;
        chaseSegmentLength = segmentLength;

        int chaseCurrentPixIdx = chaseStartPixIdx; //which color am i printing next?

        //printing pattern on the entire length based off start 
        for (var i = 0; i < m_ledBuffer.getLength(); i++){
           
            //based on current index, decide what color i should be printing 
            if (((chaseCurrentPixIdx % (chaseSegmentLength * 2))/ chaseSegmentLength) == 0){
                setPixelByID(i, seqColorOne);
            } else {
                setPixelByID(i, seqColorTwo);
            }

            chaseCurrentPixIdx += 1;
        } 

        seqTimer.start();
        commit();
    }

    //run sequence based on ledStatus
    public void executeSequence(){
        //FLASHING
        if (ledStatus == 0){
            if (seqTimer.get() >= flashInterval){
                if (currentSeqColor == seqColorOne){
                    setColor(seqColorTwo);
                    currentSeqColor = seqColorTwo;
                } else {
                    setColor(seqColorOne);
                    currentSeqColor = seqColorOne;
                }

                seqTimer.reset();
                commit();
            }

        //CHASING
        } else if (ledStatus == 1){     
            if (seqTimer.get() >= chaseInterval){
                chaseStartPixIdx += 1;
                chaseStartPixIdx %= chaseSegmentLength * 2;
                int chaseCurrentPixIdx = chaseStartPixIdx; //which color am i printing next?

                //printing pattern on the entire length based off start 
                for (var i = 0; i < m_ledBuffer.getLength(); i++){
                    
                    //based on current index, decide what color i should be printing 
                    if (((chaseCurrentPixIdx % (chaseSegmentLength * 2))/ chaseSegmentLength) == 0){
                        setPixelByID(i, seqColorOne);
                    } else {
                        setPixelByID(i, seqColorTwo);
                    }

                    chaseCurrentPixIdx += 1;
                } 

                seqTimer.reset();
                commit();
            }
        
        }
    }

    public void wakeupSequence(){
        wakeupTimer.start();

        if (wakeupTimer.get() <= 3){
            //setFlashColors(Color.kRed, Color.kBlack, 0.2);
            ledStatus = 0;
            seqColorOne = Color.kRed;
            seqColorTwo = Color.kBlack;
        
        } else if ((wakeupTimer.get() > 3) && (wakeupTimer.get() <= 6)) {
            setChasingColors(Color.kBlack, Color.kBlue, 5, 0.001);
        
        } else {
            ledStatus = 3;
            setColor(Color.kBlack);
            commit();
        }
    }

    //pass a starting hue (0-180). Will return the next hue that should be passed in if you want
    //to make it move.
    public int setRainbow(int startingHue) {
        //for every pixel
        for (var i = 0; i < m_ledBuffer.getLength(); i++) {
            //calculate the hue - hue is easier for rainbows because the color
            //shape is a circle so only one value needs to precess
            final var hue = (startingHue + (int)(i * 180.0 / m_ledBuffer.getLength())) % 180;
            
            //set the value
            m_ledBuffer.setHSV(i, hue, 255, (int)(255 * m_brightness));
        }

        startingHue += 3; //increase by to make the rainbow "move"
        startingHue %= 180; //check bounds

        return startingHue;
    }

    //commit the buffer so it's written to the pixels at the next update
    public void commit() {
        m_led.setData(m_ledBuffer);
    }
}