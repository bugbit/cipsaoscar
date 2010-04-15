#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/CIAPlayer.o \
	${OBJECTDIR}/CLocalHumanPlayer.o \
	${OBJECTDIR}/CPlayer.o \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/casciiboardrender.o \
	${OBJECTDIR}/cboard.o \
	${OBJECTDIR}/cboardrender.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/3enraya

dist/Release/GNU-Linux-x86/3enraya: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/3enraya ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/CIAPlayer.o: nbproject/Makefile-${CND_CONF}.mk CIAPlayer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CIAPlayer.o CIAPlayer.cpp

${OBJECTDIR}/CLocalHumanPlayer.o: nbproject/Makefile-${CND_CONF}.mk CLocalHumanPlayer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CLocalHumanPlayer.o CLocalHumanPlayer.cpp

${OBJECTDIR}/CPlayer.o: nbproject/Makefile-${CND_CONF}.mk CPlayer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/CPlayer.o CPlayer.cpp

${OBJECTDIR}/main.o: nbproject/Makefile-${CND_CONF}.mk main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/casciiboardrender.o: nbproject/Makefile-${CND_CONF}.mk casciiboardrender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/casciiboardrender.o casciiboardrender.cpp

${OBJECTDIR}/cboard.o: nbproject/Makefile-${CND_CONF}.mk cboard.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/cboard.o cboard.cpp

${OBJECTDIR}/cboardrender.o: nbproject/Makefile-${CND_CONF}.mk cboardrender.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/cboardrender.o cboardrender.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/3enraya

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
