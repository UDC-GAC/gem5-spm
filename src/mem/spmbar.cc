#include "mem/spmbar.hh"

using namespace std;

SpmXBar::SpmXBar(const SpmXBarParams* p) :
    NoncoherentXBar(p)
{
}


bool
SpmXBar::recvTimingReq(PacketPtr pkt, PortID slave_port_id)
{
    // determine the source port based on the id
    SlavePort *src_port = slavePorts[slave_port_id];

    // we should never see express snoops on a non-coherent crossbar
    assert(!pkt->isExpressSnoop());

    // determine the destination based on the address
    PortID master_port_id = findPort(pkt->getAddr());

    // test if the layer should be considered occupied for the current
    // port
    if (!reqLayers[master_port_id]->tryTiming(src_port)) {
        return false;
    }

    // store size and command as they might be modified when
    // forwarding the packet
    unsigned int pkt_size = pkt->hasData() ? pkt->getSize() : 0;
    unsigned int pkt_cmd = pkt->cmdToIndex();

    // store the old header delay so we can restore it if needed
    Tick old_header_delay = pkt->headerDelay;

    // a request sees the frontend and forward latency
    Tick xbar_delay = 0;

    // set the packet header and payload delay
    calcPacketTiming(pkt, xbar_delay);

    // determine how long to be crossbar layer is busy
    Tick packetFinishTime = clockEdge(Cycles(1)) + pkt->payloadDelay;

    // before forwarding the packet (and possibly altering it),
    // remember if we are expecting a response
    const bool expect_response = pkt->needsResponse() &&
        !pkt->memInhibitAsserted();

    // since it is a normal request, attempt to send the packet
    bool success = masterPorts[master_port_id]->sendTimingReq(pkt);

    if (!success)  {
        // inhibited packets should never be forced to retry
        assert(!pkt->memInhibitAsserted());

        // restore the header delay as it is additive
        pkt->headerDelay = old_header_delay;

        // occupy until the header is sent
        reqLayers[master_port_id]->failedTiming(src_port,
                                                clockEdge(Cycles(1)));

        return false;
    }

    // remember where to route the response to
    if (expect_response) {
        assert(routeTo.find(pkt->req) == routeTo.end());
        routeTo[pkt->req] = slave_port_id;
    }

    reqLayers[master_port_id]->succeededTiming(packetFinishTime);

    // stats updates
    pktCount[slave_port_id][master_port_id]++;
    pktSize[slave_port_id][master_port_id] += pkt_size;
    transDist[pkt_cmd]++;

    return true;
}

bool
SpmXBar::recvTimingResp(PacketPtr pkt, PortID master_port_id)
{
    // determine the source port based on the id
    MasterPort *src_port = masterPorts[master_port_id];

    // determine the destination
    const auto route_lookup = routeTo.find(pkt->req);
    assert(route_lookup != routeTo.end());
    const PortID slave_port_id = route_lookup->second;
    assert(slave_port_id != InvalidPortID);
    assert(slave_port_id < respLayers.size());

    // test if the layer should be considered occupied for the current
    // port
    if (!respLayers[slave_port_id]->tryTiming(src_port)) {
        return false;
    }

    // store size and command as they might be modified when
    // forwarding the packet
    unsigned int pkt_size = pkt->hasData() ? pkt->getSize() : 0;
    unsigned int pkt_cmd = pkt->cmdToIndex();

    // a response sees the response latency
    Tick xbar_delay = 0;

    // set the packet header and payload delay
    calcPacketTiming(pkt, xbar_delay);

    // determine how long to be crossbar layer is busy
    Tick packetFinishTime = clockEdge(Cycles(1)) + pkt->payloadDelay;

    // send the packet through the destination slave port
    bool success M5_VAR_USED = slavePorts[slave_port_id]->sendTimingResp(pkt);

    // currently it is illegal to block responses... can lead to
    // deadlock
    assert(success);

    // remove the request from the routing table
    routeTo.erase(route_lookup);

    respLayers[slave_port_id]->succeededTiming(packetFinishTime);

    // stats updates
    pktCount[slave_port_id][master_port_id]++;
    pktSize[slave_port_id][master_port_id] += pkt_size;
    transDist[pkt_cmd]++;

    return true;
}


void
SpmXBar::calcPacketTiming(PacketPtr pkt, Tick header_delay)
{
    // the crossbar will be called at a time that is not necessarily
    // coinciding with its own clock, so start by determining how long
    // until the next clock edge (could be zero)
    Tick offset = clockEdge() - curTick();

    // the header delay depends on the path through the crossbar, and
    // we therefore rely on the caller to provide the actual
    // value
    pkt->headerDelay += offset + header_delay;

    // note that we add the header delay to the existing value, and
    // align it to the crossbar clock

    // do a quick sanity check to ensure the timings are not being
    // ignored, note that this specific value may cause problems for
    // slower interconnects
    panic_if(pkt->headerDelay > SimClock::Int::us,
             "Encountered header delay exceeding 1 us\n");

    if (pkt->hasData()) {
        // the payloadDelay takes into account the relative time to
        // deliver the payload of the packet, after the header delay,
        // we take the maximum since the payload delay could already
        // be longer than what this parcitular crossbar enforces.
        pkt->payloadDelay = std::max<Tick>(pkt->payloadDelay,
                                           divCeil(pkt->getSize(), width) *
                                           clockPeriod());
    }

    // the payload delay is not paying for the clock offset as that is
    // already done using the header delay, and the payload delay is
    // also used to determine how long the crossbar layer is busy and
    // thus regulates throughput
}

SpmXBar *
SpmXBarParams::create()
{
    return new SpmXBar(this);
}
