/*----------------------------------------------------------------------------*/
/* Copyright (c) 2016-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

/**
 * A source that represents a video camera.
 */
public class VideoCamera extends VideoSource {
  public class WhiteBalance {
    public static final int kFixedIndoor = 3000;
    public static final int kFixedOutdoor1 = 4000;
    public static final int kFixedOutdoor2 = 5000;
    public static final int kFixedFluorescent1 = 5100;
    public static final int kFixedFlourescent2 = 5200;
  }

  protected VideoCamera(int handle) {
    super(handle);
  }

  /**
   * Set the brightness, as a percentage (0-100).
   */
  public synchronized void setBrightness(int brightness) {
    CameraServerJNI.setCameraBrightness(m_handle, brightness);
  }

  /**
   * Get the brightness, as a percentage (0-100).
   */
  public synchronized int getBrightness() {
    return CameraServerJNI.getCameraBrightness(m_handle);
  }

  /**
   * Set the white balance to auto.
   */
  public synchronized void setWhiteBalanceAuto() {
    CameraServerJNI.setCameraWhiteBalanceAuto(m_handle);
  }

  /**
   * Set the white balance to hold current.
   */
  public synchronized void setWhiteBalanceHoldCurrent() {
    CameraServerJNI.setCameraWhiteBalanceHoldCurrent(m_handle);
  }

  /**
   * Set the white balance to manual, with specified color temperature.
   */
  public synchronized void setWhiteBalanceManual(int value) {
    CameraServerJNI.setCameraWhiteBalanceManual(m_handle, value);
  }

  /**
   * Set the exposure to auto aperture.
   */
  public synchronized void setExposureAuto() {
    CameraServerJNI.setCameraExposureAuto(m_handle);
  }

  /**
   * Set the exposure to hold current.
   */
  public synchronized void setExposureHoldCurrent() {
    CameraServerJNI.setCameraExposureHoldCurrent(m_handle);
  }

  /**
   * Set the exposure to manual, as a percentage (0-100).
   */
  public synchronized void setExposureManual(int value) {
    CameraServerJNI.setCameraExposureManual(m_handle, value);
  }
}
