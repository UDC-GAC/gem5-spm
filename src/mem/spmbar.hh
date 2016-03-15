#ifndef __SPM_XBAR_HH__
#define __SPM_XBAR_HH__

#include <deque>

#include "mem/noncoherent_xbar.hh"
#include "params/NoncoherentXBar.hh"
#include "params/SpmXBar.hh"

/**
 * The base crossbar contains the common elements of the non-coherent
 * and coherent crossbar. It is an abstract class that does not have
 * any of the functionality relating to the actual reception and
 * transmission of packets, as this is left for the subclasses.
 *
 * The BaseXBar is responsible for the basic flow control (busy or
 * not), the administration of retries, and the address decoding.
 */
class SpmXBar : public NoncoherentXBar
{
  protected:
    /**
     * Calculate the timing parameters for the packet. Updates the
     * headerDelay and payloadDelay fields of the packet
     * object with the relative number of ticks required to transmit
     * the header and the payload, respectively.
     *
     * @param pkt Packet to populate with timings
     * @param header_delay Header delay to be added
     */
    void calcPacketTiming(PacketPtr pkt, Tick header_delay);
    /** Function called by the port when the crossbar is recieving a Timing
      request packet.*/
    virtual bool recvTimingReq(PacketPtr pkt, PortID slave_port_id);
  public:
       SpmXBar(const SpmXBarParams *p);
};

#endif //__SPM_XBAR_HH__
