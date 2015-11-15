#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=src/AD.c src/BOARD.c src/ES_Framework.c src/FindAmmoHSM.c src/FindOpponentHSM.c src/FindPortalHSM.c src/IO_Ports.c src/LED.c src/R2BJT2_Main.c src/R2_BJT2_HSM.c src/RC_Servo.c src/driveMotor.c src/pwm.c src/serial.c src/R2Events.c src/R2BumperService.c src/TrackWireEvents.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/BOARD.o ${OBJECTDIR}/src/ES_Framework.o ${OBJECTDIR}/src/FindAmmoHSM.o ${OBJECTDIR}/src/FindOpponentHSM.o ${OBJECTDIR}/src/FindPortalHSM.o ${OBJECTDIR}/src/IO_Ports.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/R2BJT2_Main.o ${OBJECTDIR}/src/R2_BJT2_HSM.o ${OBJECTDIR}/src/RC_Servo.o ${OBJECTDIR}/src/driveMotor.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/R2Events.o ${OBJECTDIR}/src/R2BumperService.o ${OBJECTDIR}/src/TrackWireEvents.o
POSSIBLE_DEPFILES=${OBJECTDIR}/src/AD.o.d ${OBJECTDIR}/src/BOARD.o.d ${OBJECTDIR}/src/ES_Framework.o.d ${OBJECTDIR}/src/FindAmmoHSM.o.d ${OBJECTDIR}/src/FindOpponentHSM.o.d ${OBJECTDIR}/src/FindPortalHSM.o.d ${OBJECTDIR}/src/IO_Ports.o.d ${OBJECTDIR}/src/LED.o.d ${OBJECTDIR}/src/R2BJT2_Main.o.d ${OBJECTDIR}/src/R2_BJT2_HSM.o.d ${OBJECTDIR}/src/RC_Servo.o.d ${OBJECTDIR}/src/driveMotor.o.d ${OBJECTDIR}/src/pwm.o.d ${OBJECTDIR}/src/serial.o.d ${OBJECTDIR}/src/R2Events.o.d ${OBJECTDIR}/src/R2BumperService.o.d ${OBJECTDIR}/src/TrackWireEvents.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/src/AD.o ${OBJECTDIR}/src/BOARD.o ${OBJECTDIR}/src/ES_Framework.o ${OBJECTDIR}/src/FindAmmoHSM.o ${OBJECTDIR}/src/FindOpponentHSM.o ${OBJECTDIR}/src/FindPortalHSM.o ${OBJECTDIR}/src/IO_Ports.o ${OBJECTDIR}/src/LED.o ${OBJECTDIR}/src/R2BJT2_Main.o ${OBJECTDIR}/src/R2_BJT2_HSM.o ${OBJECTDIR}/src/RC_Servo.o ${OBJECTDIR}/src/driveMotor.o ${OBJECTDIR}/src/pwm.o ${OBJECTDIR}/src/serial.o ${OBJECTDIR}/src/R2Events.o ${OBJECTDIR}/src/R2BumperService.o ${OBJECTDIR}/src/TrackWireEvents.o

# Source Files
SOURCEFILES=src/AD.c src/BOARD.c src/ES_Framework.c src/FindAmmoHSM.c src/FindOpponentHSM.c src/FindPortalHSM.c src/IO_Ports.c src/LED.c src/R2BJT2_Main.c src/R2_BJT2_HSM.c src/RC_Servo.c src/driveMotor.c src/pwm.c src/serial.c src/R2Events.c src/R2BumperService.c src/TrackWireEvents.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX320F128H
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/src/AD.o: src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/AD.o.d 
	@${RM} ${OBJECTDIR}/src/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/src/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/AD.o.d" -o ${OBJECTDIR}/src/AD.o src/AD.c   
	
${OBJECTDIR}/src/BOARD.o: src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/src/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/src/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/BOARD.o.d" -o ${OBJECTDIR}/src/BOARD.o src/BOARD.c   
	
${OBJECTDIR}/src/ES_Framework.o: src/ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/src/ES_Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/src/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/ES_Framework.o.d" -o ${OBJECTDIR}/src/ES_Framework.o src/ES_Framework.c   
	
${OBJECTDIR}/src/FindAmmoHSM.o: src/FindAmmoHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindAmmoHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindAmmoHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindAmmoHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindAmmoHSM.o.d" -o ${OBJECTDIR}/src/FindAmmoHSM.o src/FindAmmoHSM.c   
	
${OBJECTDIR}/src/FindOpponentHSM.o: src/FindOpponentHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindOpponentHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindOpponentHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindOpponentHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindOpponentHSM.o.d" -o ${OBJECTDIR}/src/FindOpponentHSM.o src/FindOpponentHSM.c   
	
${OBJECTDIR}/src/FindPortalHSM.o: src/FindPortalHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindPortalHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindPortalHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindPortalHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindPortalHSM.o.d" -o ${OBJECTDIR}/src/FindPortalHSM.o src/FindPortalHSM.c   
	
${OBJECTDIR}/src/IO_Ports.o: src/IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/src/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/src/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/IO_Ports.o.d" -o ${OBJECTDIR}/src/IO_Ports.o src/IO_Ports.c   
	
${OBJECTDIR}/src/LED.o: src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/LED.o.d 
	@${RM} ${OBJECTDIR}/src/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/src/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/LED.o.d" -o ${OBJECTDIR}/src/LED.o src/LED.c   
	
${OBJECTDIR}/src/R2BJT2_Main.o: src/R2BJT2_Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2BJT2_Main.o.d 
	@${RM} ${OBJECTDIR}/src/R2BJT2_Main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2BJT2_Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2BJT2_Main.o.d" -o ${OBJECTDIR}/src/R2BJT2_Main.o src/R2BJT2_Main.c   
	
${OBJECTDIR}/src/R2_BJT2_HSM.o: src/R2_BJT2_HSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2_BJT2_HSM.o.d 
	@${RM} ${OBJECTDIR}/src/R2_BJT2_HSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2_BJT2_HSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2_BJT2_HSM.o.d" -o ${OBJECTDIR}/src/R2_BJT2_HSM.o src/R2_BJT2_HSM.c   
	
${OBJECTDIR}/src/RC_Servo.o: src/RC_Servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/src/RC_Servo.o 
	@${FIXDEPS} "${OBJECTDIR}/src/RC_Servo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/RC_Servo.o.d" -o ${OBJECTDIR}/src/RC_Servo.o src/RC_Servo.c   
	
${OBJECTDIR}/src/driveMotor.o: src/driveMotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/driveMotor.o.d 
	@${RM} ${OBJECTDIR}/src/driveMotor.o 
	@${FIXDEPS} "${OBJECTDIR}/src/driveMotor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/driveMotor.o.d" -o ${OBJECTDIR}/src/driveMotor.o src/driveMotor.c   
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c   
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${RM} ${OBJECTDIR}/src/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/serial.o.d" -o ${OBJECTDIR}/src/serial.o src/serial.c   
	
${OBJECTDIR}/src/R2Events.o: src/R2Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2Events.o.d 
	@${RM} ${OBJECTDIR}/src/R2Events.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2Events.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2Events.o.d" -o ${OBJECTDIR}/src/R2Events.o src/R2Events.c   
	
${OBJECTDIR}/src/R2BumperService.o: src/R2BumperService.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2BumperService.o.d 
	@${RM} ${OBJECTDIR}/src/R2BumperService.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2BumperService.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2BumperService.o.d" -o ${OBJECTDIR}/src/R2BumperService.o src/R2BumperService.c   
	
${OBJECTDIR}/src/TrackWireEvents.o: src/TrackWireEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/TrackWireEvents.o.d 
	@${RM} ${OBJECTDIR}/src/TrackWireEvents.o 
	@${FIXDEPS} "${OBJECTDIR}/src/TrackWireEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/TrackWireEvents.o.d" -o ${OBJECTDIR}/src/TrackWireEvents.o src/TrackWireEvents.c   
	
else
${OBJECTDIR}/src/AD.o: src/AD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/AD.o.d 
	@${RM} ${OBJECTDIR}/src/AD.o 
	@${FIXDEPS} "${OBJECTDIR}/src/AD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/AD.o.d" -o ${OBJECTDIR}/src/AD.o src/AD.c   
	
${OBJECTDIR}/src/BOARD.o: src/BOARD.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/BOARD.o.d 
	@${RM} ${OBJECTDIR}/src/BOARD.o 
	@${FIXDEPS} "${OBJECTDIR}/src/BOARD.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/BOARD.o.d" -o ${OBJECTDIR}/src/BOARD.o src/BOARD.c   
	
${OBJECTDIR}/src/ES_Framework.o: src/ES_Framework.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/ES_Framework.o.d 
	@${RM} ${OBJECTDIR}/src/ES_Framework.o 
	@${FIXDEPS} "${OBJECTDIR}/src/ES_Framework.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/ES_Framework.o.d" -o ${OBJECTDIR}/src/ES_Framework.o src/ES_Framework.c   
	
${OBJECTDIR}/src/FindAmmoHSM.o: src/FindAmmoHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindAmmoHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindAmmoHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindAmmoHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindAmmoHSM.o.d" -o ${OBJECTDIR}/src/FindAmmoHSM.o src/FindAmmoHSM.c   
	
${OBJECTDIR}/src/FindOpponentHSM.o: src/FindOpponentHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindOpponentHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindOpponentHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindOpponentHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindOpponentHSM.o.d" -o ${OBJECTDIR}/src/FindOpponentHSM.o src/FindOpponentHSM.c   
	
${OBJECTDIR}/src/FindPortalHSM.o: src/FindPortalHSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/FindPortalHSM.o.d 
	@${RM} ${OBJECTDIR}/src/FindPortalHSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/FindPortalHSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/FindPortalHSM.o.d" -o ${OBJECTDIR}/src/FindPortalHSM.o src/FindPortalHSM.c   
	
${OBJECTDIR}/src/IO_Ports.o: src/IO_Ports.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/IO_Ports.o.d 
	@${RM} ${OBJECTDIR}/src/IO_Ports.o 
	@${FIXDEPS} "${OBJECTDIR}/src/IO_Ports.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/IO_Ports.o.d" -o ${OBJECTDIR}/src/IO_Ports.o src/IO_Ports.c   
	
${OBJECTDIR}/src/LED.o: src/LED.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/LED.o.d 
	@${RM} ${OBJECTDIR}/src/LED.o 
	@${FIXDEPS} "${OBJECTDIR}/src/LED.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/LED.o.d" -o ${OBJECTDIR}/src/LED.o src/LED.c   
	
${OBJECTDIR}/src/R2BJT2_Main.o: src/R2BJT2_Main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2BJT2_Main.o.d 
	@${RM} ${OBJECTDIR}/src/R2BJT2_Main.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2BJT2_Main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2BJT2_Main.o.d" -o ${OBJECTDIR}/src/R2BJT2_Main.o src/R2BJT2_Main.c   
	
${OBJECTDIR}/src/R2_BJT2_HSM.o: src/R2_BJT2_HSM.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2_BJT2_HSM.o.d 
	@${RM} ${OBJECTDIR}/src/R2_BJT2_HSM.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2_BJT2_HSM.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2_BJT2_HSM.o.d" -o ${OBJECTDIR}/src/R2_BJT2_HSM.o src/R2_BJT2_HSM.c   
	
${OBJECTDIR}/src/RC_Servo.o: src/RC_Servo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/RC_Servo.o.d 
	@${RM} ${OBJECTDIR}/src/RC_Servo.o 
	@${FIXDEPS} "${OBJECTDIR}/src/RC_Servo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/RC_Servo.o.d" -o ${OBJECTDIR}/src/RC_Servo.o src/RC_Servo.c   
	
${OBJECTDIR}/src/driveMotor.o: src/driveMotor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/driveMotor.o.d 
	@${RM} ${OBJECTDIR}/src/driveMotor.o 
	@${FIXDEPS} "${OBJECTDIR}/src/driveMotor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/driveMotor.o.d" -o ${OBJECTDIR}/src/driveMotor.o src/driveMotor.c   
	
${OBJECTDIR}/src/pwm.o: src/pwm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/pwm.o.d 
	@${RM} ${OBJECTDIR}/src/pwm.o 
	@${FIXDEPS} "${OBJECTDIR}/src/pwm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/pwm.o.d" -o ${OBJECTDIR}/src/pwm.o src/pwm.c   
	
${OBJECTDIR}/src/serial.o: src/serial.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/serial.o.d 
	@${RM} ${OBJECTDIR}/src/serial.o 
	@${FIXDEPS} "${OBJECTDIR}/src/serial.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/serial.o.d" -o ${OBJECTDIR}/src/serial.o src/serial.c   
	
${OBJECTDIR}/src/R2Events.o: src/R2Events.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2Events.o.d 
	@${RM} ${OBJECTDIR}/src/R2Events.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2Events.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2Events.o.d" -o ${OBJECTDIR}/src/R2Events.o src/R2Events.c   
	
${OBJECTDIR}/src/R2BumperService.o: src/R2BumperService.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/R2BumperService.o.d 
	@${RM} ${OBJECTDIR}/src/R2BumperService.o 
	@${FIXDEPS} "${OBJECTDIR}/src/R2BumperService.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/R2BumperService.o.d" -o ${OBJECTDIR}/src/R2BumperService.o src/R2BumperService.c   
	
${OBJECTDIR}/src/TrackWireEvents.o: src/TrackWireEvents.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/src 
	@${RM} ${OBJECTDIR}/src/TrackWireEvents.o.d 
	@${RM} ${OBJECTDIR}/src/TrackWireEvents.o 
	@${FIXDEPS} "${OBJECTDIR}/src/TrackWireEvents.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION)  -I"include" -MMD -MF "${OBJECTDIR}/src/TrackWireEvents.o.d" -o ${OBJECTDIR}/src/TrackWireEvents.o src/TrackWireEvents.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}           -mreserve=data@0x0:0x1FC -mreserve=boot@0x1FC02000:0x1FC02FEF -mreserve=boot@0x1FC02000:0x1FC024FF  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/R2_BJT2.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
