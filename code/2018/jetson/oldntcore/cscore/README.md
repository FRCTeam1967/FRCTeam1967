# CameraServer

The purpose of the CameraServer library is to provide a standardized, high performance, robust, and reliable method for code to access multiple cameras (either USB or IP), configure the camera settings, provide images to code, and stream either raw camera images or code processed images as M-JPEG over HTTP stream(s).

## Build Requirements
To build CameraServer, a few requirements must be met:

- Platform Native Toolchain - You must have a toolchain for your native platform installed if you wish to build CameraServer for your machine. On Windows, this is Visual Studio. On Mac, this is Clang, and on Linux, this is GCC. Your toolchain must support the `-std=c++11` language flag.
- Platform Native JDK - In order to compile CameraServer your native platform, you must have the JDK for your platform installed, so that the correct JNI headers can be included.
- ARM Toolchain - To crosscompile CameraServer for the roboRIO, you must have the FRC ARM toolchain installed, which can be found [here](http://first.wpi.edu/FRC/roborio/toolchains/).

## Building
Gradle is the main build system used by CameraServer. All tasks are run with the `gradlew` wrapper, which is included in the root of the repository. All targets that can be accomplished by Gradle are referred to as tasks. The main task available is `build`. To run Gradle, cd into the build directory and run:

```bash
./gradlew build
```

To build just the Native or ARM version, you must access the approriate subproject and build just that version. For example:

```bash
./gradlew :arm:build # Builds just the arm version of CameraServer
./gradlew :native:build # Builds just the native version of CameraServer
```

If you are building the native version on a 64 bit Linux computer, use a GCC installation which has multilib support enabled (it can compile both 32 and 64 bit programs). The package providing that support on most Linux distributions is called `gcc-multilib`.

If you do not have the ARM toolchain installed on your computer, you will run into build issues. To disable the ARM platform entirely, run with the flag `-PskipArm`, and it will be entirely skipped.

```bash
./gradlew build -PskipArm # Builds native, disables the ARM project
```

The native version of CameraServer will run tests on build. The ARM version will not, as the current platform likely does not allow running of an ARM binary.


### Custom Cross Compilers
By default, the ARM version of CameraServer uses the FRC cross compiler, which has the prefix `arm-frc-linux-gnueabi-`. If you want to cross compile with a different ARM toolchain, you can specify the `-PcompilerPrefix=prefix-string` flag. For example, to compile with `arm-linux-gnueabi-gcc`, you would run:

```bash
./gradlew :arm:build -PcompilerPrefix=arm-linux-gnueabi-
```

Keeping the full prefix is important: if you do not specify this correctly, Gradle will likely fail with an error that looks like this:

```Shell
CameraServer master* $ ./gradlew :arm:build -PcompilerPrefix=doesnotexist
Defining custom 'check' task when using the standard Gradle lifecycle plugins has been deprecated and is scheduled to be removed in Gradle 3.0
:arm:compileJava UP-TO-DATE
:arm:processResources UP-TO-DATE
:arm:classes UP-TO-DATE
:arm:jniHeadersNetworkTables UP-TO-DATE
:arm:compileNtcoreSharedLibraryNtcoreCpp FAILED

FAILURE: Build failed with an exception.

* What went wrong:
Execution failed for task ':arm:compileNtcoreSharedLibraryNtcoreCpp'.
> No tool chain is available to build for platform 'arm':
    - Tool chain 'gcc' (GNU GCC): Could not find C compiler 'gcc' in system path.
    - Tool chain 'macGcc' (Clang): Could not find C compiler 'clang' in system path.

* Try:
Run with --stacktrace option to get the stack trace. Run with --info or --debug option to get more log output.

BUILD FAILED

Total time: 2.441 secs
```

If you have the Toolchain installed somewhere not on the System PATH, you can use the `toolChainPath` property to specify where the bin location of the toolchain is installed to, for example:

```bash
./gradlew :arm:build -PtoolChainPath=some/path/to/my/toolchain/bin
```

## Testing
By default, tests will be built for the x86 and x64 versions of CameraServer, and will be run during any execution of the `build` or `publish` tasks. To skip building and running the tests, use the `-PwithoutTests` command line flag when running Gradle.

## Publishing
To use CameraServer in downstream projects as a Maven-style dependency, use the `publish` command. This will publish four artifacts, where platform_name is your current platform (windows, mac, linux):

- edu.wpi.cameraserver.cpp:CameraServer:1.0.0-SNAPSHOT:arm@zip
- edu.wpi.cameraserver.cpp:CameraServer:1.0.0-SNAPSHOT:platform_name@zip
- edu.wpi.cameraserver.java:CameraServer:1.0.0-SNAPSHOT:arm
- edu.wpi.cameraserver.java:CameraServer:1.0.0-SNAPSHOT:platform_name

These are published to ~/releases/maven/development. To publish to a different repo, specify the `-Prepo=repo_name` flag. Valid repos are:

- development - The default repo.
- beta - Publishes to ~/releases/maven/beta.
- stable - Publishes to ~/releases/maven/stable.
- release - Publishes to ~/releases/maven/release.

Most downstream projects that run on the desktop do not depend on the `platform_name` classifier version of CameraServer. Rather, they depend on a version with the `desktop` classifier. Normally, this is a version of CameraServer built by the FRC Build server that includes binaries for the 3 major operating systems (Windows, Mac, Linux) and 2 major CPU architectures (x86, x86_64). However, if you are locally testing changes to CameraServer, you may want to build a version with the `:desktop` classifier for use on your current platform. To do this, specify the `-PmakeDesktop` flag when publishing. For example:

```bash
./gradlew publish -PmakeDesktop
```

When you do a publish of CameraServer locally, regardless of whether `-PmakeDesktop` is found, the locally built copy will override all references to CameraServer dependencies from the FRC Maven server. To undo this, you must delete `~/releases/maven/<repo>/edu/wpi/cameraserver`.
