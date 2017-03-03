

#include <iostream>
#include <cstring>

#if defined(__BORLANDC__) // workaround for BCB4 release build intrinsics bug
namespace std {
using ::__strcmp__;  // avoid error: E2316 '__strcmp__' is not a member of 'std'.
}
#endif

#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
#include "ip/UdpSocket.h"
#include <sys/soundcard.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdlib.h>

//#define  MIDI_DEVICE  "/dev/snd/midiC1D0"

static int midifd = -1;  

static int baseCC = 73;


static const char* inputs[] = {
	"/1",
	"/2", 
	"/3",
	"/4",
	"/5",
	"/6",
	"/7",
	"/8",
	"/9",
	"/10",
	"/11",
	"/12",
	"/13",
	"/14",
	"/15",
	"/16",
	"/17",
	"/18",
	NULL
};


class ExamplePacketListener : public osc::OscPacketListener {
protected:

    virtual void ProcessMessage( const osc::ReceivedMessage& m, const IpEndpointName& remoteEndpoint )
    {
        (void) remoteEndpoint; // suppress unused parameter warning
        try{
            osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
            
            //osc argument
            float arg;
            args >> arg >> osc::EndMessage;
            
            //value scaling
			int value = (int)(arg * 127.0f);
			if (value > 127) {
				value = 127;
			}
			if (value < 0) {
				value = 0;
			}
			
			//routing
			const char* path = m.AddressPattern();
			int number = -1;
			int i = 0;
			while (inputs[i] != NULL) {
				if (strcmp(path, inputs[i]) == 0) {
					number = i;
					break;
				}
				i++;
			}
			
			if (number < 0) {
    			std::cout << "unmapped message " << path << " : " << arg << "\n";
    		} else if (midifd >= 0) {
	            //https://www.midi.org/specifications/item/table-1-summary-of-midi-message
            	unsigned char packet[3];
            	packet[0] = 0b10010000; //1011=cc. 1001=noteOn, 1000=noteOff 
            	packet[1] = (unsigned int) number + baseMidiCcc; //note / cc
            	packet[2] = (unsigned int) value; //(unsigned int)value
            	write(3, &packet, 3);
            	//std::cout << "midi message sent " << " " << number << " " << value << "\n";	
            }
            
        } catch( osc::Exception& e ) {
            // any parsing errors such as unexpected argument types, or 
            // missing arguments get thrown as exceptions.
            std::cout << "error while parsing message: " << m.AddressPattern() << ": " << e.what() << "\n";
        }
    }
    
};



int main(int argc, char* argv[]) {
	
	(void) argc; // suppress unused parameter warnings
    (void) argv; // suppress unused parameter warnings
     
    if (argc!=3) {
    	printf("biomusic osc to midi mapper.\n usage : %s /dev/snd/<midiDevice> <oscPort>\n", argv[0]);
    	exit(1);
    }
    
    //midi device
    char* device = argv[1];
    
    //port
    int port = atoi(argv[2]);
    
    
    printf("biomusic osc to midi mapper\n");
    printf("mappings:\n");
    int i = 0;
    while (inputs[i] != NULL) {
		printf("  %i %s\n", i, inputs[i]);
		i++;
	}
    
   // open midi device
   midifd = open(device, O_WRONLY, 0);
   if (midifd < 0) {
      printf("Error: cannot open %s\n", device);
      exit(1);
   } else {
	   std::cout << "opened midi device " << device << "\n";
   }
   std::cout << "starting osc on port " << port <<"\n";
   
   	//start server
    ExamplePacketListener listener;
    UdpListeningReceiveSocket s( IpEndpointName( IpEndpointName::ANY_ADDRESS, 5000 ), &listener );
	
    s.RunUntilSigInt();

    return 0;
}

