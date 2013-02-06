# Wind River Workbench generated Makefile.
# Do not edit!!!
#
# The file ".wrmakefile" is the template used by the Wind River Workbench to
# generate the makefiles of this project. Add user-specific build targets and
# make rules only(!) in this project's ".wrmakefile" file. These will then be
# automatically dumped into the makefiles.

WIND_HOME := $(subst \,/,$(WIND_HOME))
WIND_BASE := $(subst \,/,$(WIND_BASE))
WIND_USR := $(subst \,/,$(WIND_USR))

all : pre_build main_all post_build

_clean ::
	@echo "make: removing targets and objects of `pwd`"

TRACE=0
TRACEON=$(TRACE:0=@)
TRACE_FLAG=$(TRACEON:1=)

MAKEFILE := Makefile

FLEXIBLE_BUILD := 1

BUILD_SPEC = PPC603gnu
DEBUG_MODE = 1
ifeq ($(DEBUG_MODE),1)
MODE_DIR := Debug
else
MODE_DIR := NonDebug
endif
OBJ_DIR := .
PRJ_ROOT_DIR := C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib
WS_ROOT_DIR := C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code



#Global Build Macros
PROJECT_TYPE = DKM
DEFINES = 
EXPAND_DBG = 0


#BuildSpec specific Build Macros
VX_CPU_FAMILY = ppc
CPU = PPC603
TOOL_FAMILY = gnu
TOOL = gnu
TOOL_PATH = 
CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
LIBPATH = 
LIBS = 

IDE_INCLUDES = -I.. -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 

IDE_LIBRARIES = 



#BuildTool flags
ifeq ($(DEBUG_MODE),1)
DEBUGFLAGS_C-Compiler = -g
DEBUGFLAGS_C++-Compiler = -g
DEBUGFLAGS_Linker = -g
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -g
else
DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
DEBUGFLAGS_Partial-Image-Linker = 
DEBUGFLAGS_Librarian = 
DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif


#Project Targets
PROJECT_TARGETS = WPILib/$(MODE_DIR)/WPILib.a \
	 JavaCameraLib/$(MODE_DIR)/JavaCameraLib.a


#Rules

# WPILib
ifeq ($(DEBUG_MODE),1)
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
WPILib/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
WPILib/$(MODE_DIR)/% : IDE_INCLUDES = -I.. -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
WPILib/$(MODE_DIR)/% : IDE_LIBRARIES = 
WPILib/$(MODE_DIR)/% : PROJECT_TYPE = DKM
WPILib/$(MODE_DIR)/% : DEFINES = 
WPILib/$(MODE_DIR)/% : EXPAND_DBG = 0
WPILib/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
WPILib/$(MODE_DIR)/% : CPU = PPC603
WPILib/$(MODE_DIR)/% : TOOL_FAMILY = gnu
WPILib/$(MODE_DIR)/% : TOOL = gnu
WPILib/$(MODE_DIR)/% : TOOL_PATH = 
WPILib/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
WPILib/$(MODE_DIR)/% : LIBPATH = 
WPILib/$(MODE_DIR)/% : LIBS = 
WPILib/$(MODE_DIR)/% : OBJ_DIR := WPILib/$(MODE_DIR)

WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_I2C.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/ADXL345_I2C.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_SPI.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/ADXL345_SPI.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Accelerometer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Accelerometer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/AnalogChannel.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/AnalogChannel.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/AnalogModule.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/AnalogModule.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTrigger.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/AnalogTrigger.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTriggerOutput.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/AnalogTriggerOutput.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/AnalogIOButton.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/AnalogIOButton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Button.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/Button.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ButtonScheduler.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/ButtonScheduler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/DigitalIOButton.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/DigitalIOButton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/HeldButtonScheduler.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/HeldButtonScheduler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/InternalButton.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/InternalButton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/JoystickButton.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/JoystickButton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/NetworkButton.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/NetworkButton.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/PressedButtonScheduler.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/PressedButtonScheduler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ReleasedButtonScheduler.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/ReleasedButtonScheduler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Trigger.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Buttons/Trigger.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/CANJaguar.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/CANJaguar.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Command.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/Command.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroup.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/CommandGroup.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroupEntry.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/CommandGroupEntry.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDCommand.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/PIDCommand.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDSubsystem.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/PIDSubsystem.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PrintCommand.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/PrintCommand.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Scheduler.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/Scheduler.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/StartCommand.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/StartCommand.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Subsystem.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/Subsystem.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitCommand.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/WaitCommand.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitForChildren.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/WaitForChildren.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitUntilCommand.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Commands/WaitUntilCommand.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Compressor.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Compressor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Counter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Counter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Dashboard.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Dashboard.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DigitalInput.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DigitalInput.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DigitalModule.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DigitalModule.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DigitalOutput.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DigitalOutput.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DigitalSource.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DigitalSource.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DoubleSolenoid.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DoubleSolenoid.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DriverStation.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DriverStation.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationEnhancedIO.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DriverStationEnhancedIO.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationLCD.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/DriverStationLCD.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Encoder.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Encoder.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Error.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Error.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/ErrorBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/ErrorBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/GearTooth.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/GearTooth.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Gyro.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Gyro.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicColorSensor.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/HiTechnicColorSensor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicCompass.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/HiTechnicCompass.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/I2C.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/I2C.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/InterruptableSensorBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/InterruptableSensorBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/IterativeRobot.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/IterativeRobot.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Jaguar.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Jaguar.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Joystick.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Joystick.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Kinect.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Kinect.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/KinectStick.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/KinectStick.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindow.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/LiveWindow/LiveWindow.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindowStatusListener.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/LiveWindow/LiveWindowStatusListener.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Module.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Module.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/MotorSafetyHelper.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/MotorSafetyHelper.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Notifier.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Notifier.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/PIDController.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/PIDController.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/PWM.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/PWM.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Preferences.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Preferences.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Relay.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Relay.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Resource.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Resource.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/RobotBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/RobotBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/RobotDrive.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/RobotDrive.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SPI.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SPI.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SafePWM.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SafePWM.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SensorBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SensorBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SerialPort.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SerialPort.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Servo.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Servo.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SimpleRobot.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SimpleRobot.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SendableChooser.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SmartDashboard/SendableChooser.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SmartDashboard.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SmartDashboard/SmartDashboard.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Solenoid.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Solenoid.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/SolenoidBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/SolenoidBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Synchronized.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Synchronized.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Talon.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Talon.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Task.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Task.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Timer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Timer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Ultrasonic.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Ultrasonic.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Utility.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Utility.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Victor.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Victor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCamera.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/AxisCamera.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCameraParams.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/AxisCameraParams.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/BinaryImage.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/BinaryImage.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ColorImage.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/ColorImage.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/EnumCameraParameter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/EnumCameraParameter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/HSLImage.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/HSLImage.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ImageBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/ImageBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/IntCameraParameter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/IntCameraParameter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/MonoImage.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/MonoImage.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/PCVideoServer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/PCVideoServer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/RGBImage.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/RGBImage.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision/Threshold.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/Threshold.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/AxisCamera.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision2009/AxisCamera.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/BaeUtilities.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision2009/BaeUtilities.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/FrcError.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision2009/FrcError.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/TrackAPI.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision2009/TrackAPI.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/VisionAPI.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision2009/VisionAPI.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/Watchdog.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Watchdog.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTable.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTable.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableConnectionListenerAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableConnectionListenerAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableKeyListenerAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableKeyListenerAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableListenerAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableListenerAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableMode.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableMode.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableProvider.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableProvider.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableSubListenerAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables/NetworkTableSubListenerAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/AbstractNetworkTableEntryStore.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/AbstractNetworkTableEntryStore.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableEntry.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/NetworkTableEntry.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableNode.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/NetworkTableNode.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/OutgoingEntryReciever.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/OutgoingEntryReciever.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TableKeyExistsWithDifferentTypeException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/TableKeyExistsWithDifferentTypeException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TransactionDirtier.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/TransactionDirtier.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/WriteManager.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/WriteManager.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/client/ClientConnectionAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionState.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/client/ClientConnectionState.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientNetworkTableEntryStore.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/client/ClientNetworkTableEntryStore.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/NetworkTableClient.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/client/NetworkTableClient.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/BadMessageException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/connection/BadMessageException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/ConnectionMonitorThread.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/connection/ConnectionMonitorThread.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/DataIOStream.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/connection/DataIOStream.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/NetworkTableConnection.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/connection/NetworkTableConnection.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/NetworkTableServer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/NetworkTableServer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionAdapter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/ServerConnectionAdapter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionList.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/ServerConnectionList.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionState.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/ServerConnectionState.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerIncomingStreamMonitor.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/ServerIncomingStreamMonitor.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerNetworkTableEntryStore.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/server/ServerNetworkTableEntryStore.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/FDIOStream.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/stream/FDIOStream.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketServerStreamProvider.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/stream/SocketServerStreamProvider.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreamFactory.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/stream/SocketStreamFactory.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreams.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/stream/SocketStreams.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/thread/DefaultThreadManger.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/thread/DefaultThreadManger.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayData.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/ArrayData.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayEntryType.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/ArrayEntryType.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/BooleanArray.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/BooleanArray.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexData.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/ComplexData.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexEntryType.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/ComplexEntryType.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/DefaultEntryTypes.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/DefaultEntryTypes.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryType.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/NetworkTableEntryType.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryTypeManager.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/NetworkTableEntryTypeManager.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NumberArray.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/NumberArray.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/StringArray.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/type/StringArray.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/EOFException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/util/EOFException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IOException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/util/IOException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IllegalStateException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/util/IllegalStateException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/StringCache.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/util/StringCache.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/System.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/networktables2/util/System.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


WPILib/$(MODE_DIR)/Objects/WPILib/tables/TableKeyNotDefinedException.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/tables/TableKeyNotDefinedException.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


OBJECTS_WPILib = WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_I2C.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_SPI.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Accelerometer.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogChannel.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogModule.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTrigger.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTriggerOutput.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/AnalogIOButton.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Button.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ButtonScheduler.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/DigitalIOButton.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/HeldButtonScheduler.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/InternalButton.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/JoystickButton.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/NetworkButton.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/PressedButtonScheduler.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ReleasedButtonScheduler.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Trigger.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/CANJaguar.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Command.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroup.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroupEntry.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDCommand.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDSubsystem.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PrintCommand.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Scheduler.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/StartCommand.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Subsystem.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitCommand.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitForChildren.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitUntilCommand.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Compressor.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Counter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Dashboard.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DigitalInput.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DigitalModule.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DigitalOutput.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DigitalSource.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DoubleSolenoid.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DriverStation.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationEnhancedIO.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationLCD.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Encoder.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Error.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/ErrorBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/GearTooth.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Gyro.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicColorSensor.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicCompass.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/I2C.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/InterruptableSensorBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/IterativeRobot.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Jaguar.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Joystick.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Kinect.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/KinectStick.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindow.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindowStatusListener.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Module.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/MotorSafetyHelper.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Notifier.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/PIDController.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/PWM.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Preferences.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Relay.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Resource.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/RobotBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/RobotDrive.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SPI.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SafePWM.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SensorBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SerialPort.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Servo.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SimpleRobot.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SendableChooser.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SmartDashboard.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Solenoid.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SolenoidBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Synchronized.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Talon.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Task.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Timer.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Ultrasonic.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Utility.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Victor.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCamera.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCameraParams.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/BinaryImage.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ColorImage.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/EnumCameraParameter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/HSLImage.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ImageBase.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/IntCameraParameter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/MonoImage.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/PCVideoServer.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/RGBImage.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/Threshold.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/AxisCamera.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/BaeUtilities.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/FrcError.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/TrackAPI.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/VisionAPI.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Watchdog.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTable.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableConnectionListenerAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableKeyListenerAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableListenerAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableMode.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableProvider.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableSubListenerAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/AbstractNetworkTableEntryStore.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableEntry.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableNode.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/OutgoingEntryReciever.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TableKeyExistsWithDifferentTypeException.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TransactionDirtier.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/WriteManager.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionState.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientNetworkTableEntryStore.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/NetworkTableClient.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/BadMessageException.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/ConnectionMonitorThread.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/DataIOStream.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/NetworkTableConnection.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/NetworkTableServer.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionAdapter.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionList.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionState.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerIncomingStreamMonitor.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerNetworkTableEntryStore.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/FDIOStream.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketServerStreamProvider.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreamFactory.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreams.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/thread/DefaultThreadManger.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayData.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayEntryType.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/BooleanArray.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexData.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexEntryType.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/DefaultEntryTypes.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryType.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryTypeManager.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NumberArray.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/StringArray.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/EOFException.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IOException.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IllegalStateException.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/StringCache.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/System.o \
	 WPILib/$(MODE_DIR)/Objects/WPILib/tables/TableKeyNotDefinedException.o

WPILib/$(MODE_DIR)/WPILib.a : $(OBJECTS_WPILib)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)arppc crus "$@" $(OBJECTS_WPILib)

WPILib/$(MODE_DIR)/WPILib_compile_file : $(FILE) ;

_clean :: WPILib/$(MODE_DIR)/WPILib_clean

WPILib/$(MODE_DIR)/WPILib_clean : 
	$(TRACE_FLAG)if [ -d "WPILib" ]; then cd "WPILib"; rm -rf $(MODE_DIR); fi


# JavaCameraLib
ifeq ($(DEBUG_MODE),1)
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -g
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -g
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -g
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -g
else
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_C-Compiler = -O2 -fstrength-reduce -fno-builtin
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_C++-Compiler = -O2 -fstrength-reduce -fno-builtin
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Linker = -O2 -fstrength-reduce -fno-builtin
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Partial-Image-Linker = 
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Librarian = 
JavaCameraLib/$(MODE_DIR)/% : DEBUGFLAGS_Assembler = -O2 -fstrength-reduce -fno-builtin
endif
JavaCameraLib/$(MODE_DIR)/% : IDE_INCLUDES = -I.. -I$(WIND_BASE)/target/h -I$(WIND_BASE)/target/h/wrn/coreip 
JavaCameraLib/$(MODE_DIR)/% : IDE_LIBRARIES = 
JavaCameraLib/$(MODE_DIR)/% : PROJECT_TYPE = DKM
JavaCameraLib/$(MODE_DIR)/% : DEFINES = 
JavaCameraLib/$(MODE_DIR)/% : EXPAND_DBG = 0
JavaCameraLib/$(MODE_DIR)/% : VX_CPU_FAMILY = ppc
JavaCameraLib/$(MODE_DIR)/% : CPU = PPC603
JavaCameraLib/$(MODE_DIR)/% : TOOL_FAMILY = gnu
JavaCameraLib/$(MODE_DIR)/% : TOOL = gnu
JavaCameraLib/$(MODE_DIR)/% : TOOL_PATH = 
JavaCameraLib/$(MODE_DIR)/% : CC_ARCH_SPEC = -mcpu=603 -mstrict-align -mno-implicit-fp -mlongcall
JavaCameraLib/$(MODE_DIR)/% : LIBPATH = 
JavaCameraLib/$(MODE_DIR)/% : LIBS = 
JavaCameraLib/$(MODE_DIR)/% : OBJ_DIR := JavaCameraLib/$(MODE_DIR)

JavaCameraLib/$(MODE_DIR)/Objects/PCVideoServer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/PCVideoServer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/AxisCamera.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/AxisCamera.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/AxisCameraParams.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/AxisCameraParams.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/EnumCameraParameter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/EnumCameraParameter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/IntCameraParameter.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Vision/IntCameraParameter.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/Error.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Error.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/ErrorBase.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/ErrorBase.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/Task.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Task.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/Timer.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Timer.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/Synchronized.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Synchronized.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


JavaCameraLib/$(MODE_DIR)/Objects/Utility.o : C:/Documents\ and\ Settings/Team1967/My\ Documents/GitHub/FRCTeam1967/code/WPILib_Sources/WPILib/Utility.cpp
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi; $(TOOL_PATH)ccppc $(DEBUGFLAGS_C++-Compiler) $(CC_ARCH_SPEC) -ansi -Wall  -MD -MP $(ADDED_C++FLAGS) $(IDE_INCLUDES) $(ADDED_INCLUDES) -DCPU=$(CPU) -DTOOL_FAMILY=$(TOOL_FAMILY) -DTOOL=$(TOOL) -DJAVA_CAMERA_LIB=1 -D_WRS_KERNEL -D'SVN_REV="$(shell svnversion -n ..)"' $(DEFINES) -o "$@" -c "$<"


OBJECTS_JavaCameraLib = JavaCameraLib/$(MODE_DIR)/Objects/PCVideoServer.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/AxisCamera.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/AxisCameraParams.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/EnumCameraParameter.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/IntCameraParameter.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/Error.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/ErrorBase.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/Task.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/Timer.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/Synchronized.o \
	 JavaCameraLib/$(MODE_DIR)/Objects/Utility.o

JavaCameraLib/$(MODE_DIR)/JavaCameraLib.a : $(OBJECTS_JavaCameraLib)
	$(TRACE_FLAG)if [ ! -d "`dirname "$@"`" ]; then mkdir -p "`dirname "$@"`"; fi;echo "building $@"; $(TOOL_PATH)arppc crus "$@" $(OBJECTS_JavaCameraLib)

JavaCameraLib/$(MODE_DIR)/JavaCameraLib_compile_file : $(FILE) ;

_clean :: JavaCameraLib/$(MODE_DIR)/JavaCameraLib_clean

JavaCameraLib/$(MODE_DIR)/JavaCameraLib_clean : 
	$(TRACE_FLAG)if [ -d "JavaCameraLib" ]; then cd "JavaCameraLib"; rm -rf $(MODE_DIR); fi

DEP_FILES := WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_I2C.d WPILib/$(MODE_DIR)/Objects/WPILib/ADXL345_SPI.d WPILib/$(MODE_DIR)/Objects/WPILib/Accelerometer.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogChannel.d WPILib/$(MODE_DIR)/Objects/WPILib/AnalogModule.d WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTrigger.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/AnalogTriggerOutput.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/AnalogIOButton.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Button.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ButtonScheduler.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/DigitalIOButton.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/HeldButtonScheduler.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/InternalButton.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/JoystickButton.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/NetworkButton.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/PressedButtonScheduler.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/ReleasedButtonScheduler.d WPILib/$(MODE_DIR)/Objects/WPILib/Buttons/Trigger.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/CANJaguar.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Command.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroup.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/CommandGroupEntry.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDCommand.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PIDSubsystem.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/PrintCommand.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Scheduler.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/StartCommand.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/Subsystem.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitCommand.d WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitForChildren.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Commands/WaitUntilCommand.d WPILib/$(MODE_DIR)/Objects/WPILib/Compressor.d WPILib/$(MODE_DIR)/Objects/WPILib/Counter.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Dashboard.d WPILib/$(MODE_DIR)/Objects/WPILib/DigitalInput.d WPILib/$(MODE_DIR)/Objects/WPILib/DigitalModule.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DigitalOutput.d WPILib/$(MODE_DIR)/Objects/WPILib/DigitalSource.d WPILib/$(MODE_DIR)/Objects/WPILib/DoubleSolenoid.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/DriverStation.d WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationEnhancedIO.d WPILib/$(MODE_DIR)/Objects/WPILib/DriverStationLCD.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Encoder.d WPILib/$(MODE_DIR)/Objects/WPILib/Error.d WPILib/$(MODE_DIR)/Objects/WPILib/ErrorBase.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/GearTooth.d WPILib/$(MODE_DIR)/Objects/WPILib/Gyro.d WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicColorSensor.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/HiTechnicCompass.d WPILib/$(MODE_DIR)/Objects/WPILib/I2C.d WPILib/$(MODE_DIR)/Objects/WPILib/InterruptableSensorBase.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/IterativeRobot.d WPILib/$(MODE_DIR)/Objects/WPILib/Jaguar.d WPILib/$(MODE_DIR)/Objects/WPILib/Joystick.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Kinect.d WPILib/$(MODE_DIR)/Objects/WPILib/KinectStick.d WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindow.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/LiveWindow/LiveWindowStatusListener.d WPILib/$(MODE_DIR)/Objects/WPILib/Module.d WPILib/$(MODE_DIR)/Objects/WPILib/MotorSafetyHelper.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Notifier.d WPILib/$(MODE_DIR)/Objects/WPILib/PIDController.d WPILib/$(MODE_DIR)/Objects/WPILib/PWM.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Preferences.d WPILib/$(MODE_DIR)/Objects/WPILib/Relay.d WPILib/$(MODE_DIR)/Objects/WPILib/Resource.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/RobotBase.d WPILib/$(MODE_DIR)/Objects/WPILib/RobotDrive.d WPILib/$(MODE_DIR)/Objects/WPILib/SPI.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SafePWM.d WPILib/$(MODE_DIR)/Objects/WPILib/SensorBase.d WPILib/$(MODE_DIR)/Objects/WPILib/SerialPort.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Servo.d WPILib/$(MODE_DIR)/Objects/WPILib/SimpleRobot.d WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SendableChooser.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/SmartDashboard/SmartDashboard.d WPILib/$(MODE_DIR)/Objects/WPILib/Solenoid.d WPILib/$(MODE_DIR)/Objects/WPILib/SolenoidBase.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Synchronized.d WPILib/$(MODE_DIR)/Objects/WPILib/Talon.d WPILib/$(MODE_DIR)/Objects/WPILib/Task.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Timer.d WPILib/$(MODE_DIR)/Objects/WPILib/Ultrasonic.d WPILib/$(MODE_DIR)/Objects/WPILib/Utility.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Victor.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCamera.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/AxisCameraParams.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/BinaryImage.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ColorImage.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/EnumCameraParameter.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/HSLImage.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/ImageBase.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/IntCameraParameter.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/MonoImage.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/PCVideoServer.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision/RGBImage.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision/Threshold.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/AxisCamera.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/BaeUtilities.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/FrcError.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/TrackAPI.d WPILib/$(MODE_DIR)/Objects/WPILib/Vision2009/VisionAPI.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/Watchdog.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTable.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableConnectionListenerAdapter.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableKeyListenerAdapter.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableListenerAdapter.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableMode.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableProvider.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables/NetworkTableSubListenerAdapter.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/AbstractNetworkTableEntryStore.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableEntry.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/NetworkTableNode.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/OutgoingEntryReciever.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TableKeyExistsWithDifferentTypeException.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/TransactionDirtier.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/WriteManager.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionAdapter.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientConnectionState.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/ClientNetworkTableEntryStore.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/client/NetworkTableClient.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/BadMessageException.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/ConnectionMonitorThread.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/DataIOStream.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/connection/NetworkTableConnection.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/NetworkTableServer.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionAdapter.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionList.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerConnectionState.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerIncomingStreamMonitor.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/server/ServerNetworkTableEntryStore.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/FDIOStream.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketServerStreamProvider.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreamFactory.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/stream/SocketStreams.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/thread/DefaultThreadManger.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayData.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ArrayEntryType.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/BooleanArray.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexData.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/ComplexEntryType.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/DefaultEntryTypes.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryType.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NetworkTableEntryTypeManager.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/NumberArray.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/type/StringArray.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/EOFException.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IOException.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/IllegalStateException.d WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/StringCache.d \
	 WPILib/$(MODE_DIR)/Objects/WPILib/networktables2/util/System.d WPILib/$(MODE_DIR)/Objects/WPILib/tables/TableKeyNotDefinedException.d JavaCameraLib/$(MODE_DIR)/Objects/PCVideoServer.d \
	 JavaCameraLib/$(MODE_DIR)/Objects/AxisCamera.d JavaCameraLib/$(MODE_DIR)/Objects/AxisCameraParams.d JavaCameraLib/$(MODE_DIR)/Objects/EnumCameraParameter.d \
	 JavaCameraLib/$(MODE_DIR)/Objects/IntCameraParameter.d JavaCameraLib/$(MODE_DIR)/Objects/Error.d JavaCameraLib/$(MODE_DIR)/Objects/ErrorBase.d \
	 JavaCameraLib/$(MODE_DIR)/Objects/Task.d JavaCameraLib/$(MODE_DIR)/Objects/Timer.d JavaCameraLib/$(MODE_DIR)/Objects/Synchronized.d \
	 JavaCameraLib/$(MODE_DIR)/Objects/Utility.d
-include $(DEP_FILES)



-include $(PRJ_ROOT_DIR)/*.makefile

-include *.makefile

main_all : external_build $(PROJECT_TARGETS)
	@echo "make: built targets of `pwd`"

# entry point for extending the build
external_build ::
	@echo ""

# main entry point for pre processing prior to the build
pre_build :: $(PRE_BUILD_STEP) generate_sources
	@echo ""

# entry point for generating sources prior to the build
generate_sources ::
	@echo ""

# main entry point for post processing after the build
post_build :: $(POST_BUILD_STEP) deploy_output
	@echo ""

# entry point for deploying output after the build
deploy_output ::
	@echo ""

clean :: external_clean $(CLEAN_STEP) _clean

# entry point for extending the build clean
external_clean ::
	@echo ""
