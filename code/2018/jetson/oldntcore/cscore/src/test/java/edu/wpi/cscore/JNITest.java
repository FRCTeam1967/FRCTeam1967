/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

package edu.wpi.cscore;

import org.junit.Test;

public class JNITest {
    @Test
    public void jniLinkTest() {
        // Test to verify that the JNI test link works correctly.
        edu.wpi.cscore.CameraServerJNI.getHostname();
    }
}
