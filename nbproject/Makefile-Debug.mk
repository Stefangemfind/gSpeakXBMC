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
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/21457980/config.o \
	${OBJECTDIR}/_ext/21457980/interpret.o \
	${OBJECTDIR}/_ext/21457980/listen.o \
	${OBJECTDIR}/_ext/21457980/speak.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-lsphinxbase -lpocketsphinx -lsphinxad -lasound
CXXFLAGS=-lsphinxbase -lpocketsphinx -lsphinxad -lasound

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=/opt/festival-2.1/festival/src/lib/libFestival.a /opt/festival-2.1/speech_tools/lib/libestools.a /opt/festival-2.1/speech_tools/lib/libestbase.a /opt/festival-2.1/speech_tools/lib/libeststring.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc: /opt/festival-2.1/festival/src/lib/libFestival.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc: /opt/festival-2.1/speech_tools/lib/libestools.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc: /opt/festival-2.1/speech_tools/lib/libestbase.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc: /opt/festival-2.1/speech_tools/lib/libeststring.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc ${OBJECTFILES} ${LDLIBSOPTIONS} -lasound -lcurses -lcurl

${OBJECTDIR}/_ext/21457980/config.o: /root/NetBeansProjects/gSpeakXBMC/config.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/21457980
	${RM} $@.d
	$(COMPILE.cc) -g -I/opt/festival-2.1/festival/src/include -I/opt/festival-2.1/speech_tools/include -lsphinxbase -lpocketsphinx -lsphinxad -lasound -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/21457980/config.o /root/NetBeansProjects/gSpeakXBMC/config.cpp

${OBJECTDIR}/_ext/21457980/interpret.o: /root/NetBeansProjects/gSpeakXBMC/interpret.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/21457980
	${RM} $@.d
	$(COMPILE.cc) -g -I/opt/festival-2.1/festival/src/include -I/opt/festival-2.1/speech_tools/include -lsphinxbase -lpocketsphinx -lsphinxad -lasound -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/21457980/interpret.o /root/NetBeansProjects/gSpeakXBMC/interpret.cpp

${OBJECTDIR}/_ext/21457980/listen.o: /root/NetBeansProjects/gSpeakXBMC/listen.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/21457980
	${RM} $@.d
	$(COMPILE.cc) -g -I/opt/festival-2.1/festival/src/include -I/opt/festival-2.1/speech_tools/include -lsphinxbase -lpocketsphinx -lsphinxad -lasound -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/21457980/listen.o /root/NetBeansProjects/gSpeakXBMC/listen.cpp

${OBJECTDIR}/_ext/21457980/speak.o: /root/NetBeansProjects/gSpeakXBMC/speak.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/21457980
	${RM} $@.d
	$(COMPILE.cc) -g -I/opt/festival-2.1/festival/src/include -I/opt/festival-2.1/speech_tools/include -lsphinxbase -lpocketsphinx -lsphinxad -lasound -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/21457980/speak.o /root/NetBeansProjects/gSpeakXBMC/speak.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I/opt/festival-2.1/festival/src/include -I/opt/festival-2.1/speech_tools/include -lsphinxbase -lpocketsphinx -lsphinxad -lasound -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/gspeakxbmc

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
