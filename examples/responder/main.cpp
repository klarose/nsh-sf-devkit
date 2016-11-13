//! This program provides a service function that responds to TCP SYN packets with RST packets.
//! This uses an algorithmic reversal.

#include "Responder.h"
#include <NetInterfaceFactory/NetInterfaceFactory.h>
#include <Interfaces/NetInterface.h>
#include <Demux/Demux.h>
#include <Scheduler/Scheduler.h>
#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        auto interface = nshdev::NetInterfaceFactory::CreateInterfaceFromArgs(argc, argv);
        if(interface == nullptr)
        {
            std::cerr << "Failed to create interface" << std::endl;
            return 2;
        }

        nshdev::Demux demux;
        interface->SetConsumer(&demux);

        Responder responder;
	responder.AddAllowPort(80);   // todo make configurable
        demux.SetConsumer(&responder);

        nshdev::Scheduler scheduler(*interface);

        scheduler.SchedulePackets();

    }
    catch(const std::exception& e)
    {
        std::cerr << "Caught exception (aborting)" << std::endl << e.what() << std::endl;
    }

    return 1;  // can only get here by exception
}
