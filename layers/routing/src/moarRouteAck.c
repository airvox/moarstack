//
// Created by spiralis on 29.10.16.
//

#include <moarRoutingStoredPacket.h>
#include <moarRoutingPrivate.h>
#include <moarRouteAck.h>
#include "moarRouteAck.h"


int produceAck(RoutingLayer_T *layer, RouteStoredPacket_T* original) {
    if (NULL == layer || NULL == original)
        return FUNC_RESULT_FAILED_ARGUMENT;

    RouteStoredPacket_T packet = {0};
    packet.Destination = original->Source;
    packet.MessageId = original->MessageId;
    midGenerate(&packet.InternalId, MoarLayer_Routing);

    packet.Source = layer->LocalAddress;
    packet.NextProcessing = 0;
    packet.PackType = RoutePackType_Ack;
    packet.State = StoredPackState_InProcessing;
    packet.TrysLeft = DEFAULT_ROUTE_TRYS;
    packet.PayloadSize = sizeof(RoutePayloadAck_T);
    packet.Payload = malloc(packet.PayloadSize);
    if (NULL == packet.Payload) return FUNC_RESULT_FAILED_MEM_ALLOCATION;

    RoutePayloadAck_T* payload = (RoutePayloadAck_T*)packet.Payload;
    payload->originalDestination = layer->LocalAddress;
    payload->originalSource = original->Source;
    
    return psAdd(&layer->PacketStorage, &packet);
}
