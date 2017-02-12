# oscpack makefile

# the source code should auto-detect endianess for most systems
# (see osc/OscHostEndianness.h)
# otherwise you need to explicitly set ENDIANESS below
# to either OSC_HOST_BIG_ENDIAN or OSC_HOST_LITTLE_ENDIAN
# Apple Mac OS X (PowerPC): OSC_HOST_BIG_ENDIAN
# Apple Mac OS X (Intel): OSC_HOST_LITTLE_ENDIAN
# Win32: OSC_HOST_LITTLE_ENDIAN
# i386 GNU/Linux: OSC_HOST_LITTLE_ENDIAN

ENDIANESS=OSC_DETECT_ENDIANESS #source code will detect using preprocessor
#ENDIANESS=OSC_HOST_LITTLE_ENDIAN

UNAME := $(shell uname)

CXX := g++
INCLUDES := -I.
COPTS  := -Wall -Wextra -O3
CDEBUG := -Wall -Wextra -g 
CXXFLAGS := $(COPTS) $(INCLUDES) -D$(ENDIANESS)

BINDIR := bin

#Name definitions
BIOMUSIC := $(BINDIR)/biomusic

# Common source groups

RECEIVESOURCES := osc/OscReceivedElements.cpp osc/OscPrintReceivedElements.cpp
SENDSOURCES := osc/OscOutboundPacketStream.cpp
NETSOURCES := ip/posix/UdpSocket.cpp ip/IpEndpointName.cpp ip/posix/NetworkingUtils.cpp
OSCPACKSOURCES := osc/OscTypes.cpp

BIOMUSICSOURCES := Biomusic.cpp

RECEIVEOBJECTS := $(RECEIVESOURCES:.cpp=.o)
SENDOBJECTS := $(SENDSOURCES:.cpp=.o)
NETOBJECTS := $(NETSOURCES:.cpp=.o)
OSCPACKOBJECTS := $(OSCPACKSOURCES:.cpp=.o)

BIOMUSICOBJECTS := $(BIOMUSICSOURCES:.cpp=.o)

.PHONY: all  biomusic clean

all: biomusic     

biomusic : $(BIOMUSIC)

$(BIOMUSIC) : $(BIOMUSICOBJECTS) $(OSCPACKOBJECTS) $(RECEIVEOBJECTS) $(NETOBJECTS) | $(BINDIR)
	$(CXX) -o $@ $^

$(BINDIR):
	mkdir $@

clean:
	rm -rf $(BINDIR) $(BIOMUSICOBJECTS) $(OSCPACKOBJECTS) $(NETOBJECTS) &> /dev/null



