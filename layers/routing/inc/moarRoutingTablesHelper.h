//
// Created by svalov on 10/5/16.
//

#ifndef MOARSTACK_MOARROUTINGTABLESHANDLER_H
#define MOARSTACK_MOARROUTINGTABLESHANDLER_H

#include <moarRoutingNeighborsStorage.h>
#include <moarRouting.h>
#include <moarRoutingPrivate.h>

int helperAddRoute(RoutingLayer_T* layer, RouteAddr_T* dest, RouteAddr_T* relay);
int helperAddNeighbor(RoutingLayer_T* layer, ChannelAddr_T* address);
int helperRemoveRoute(RoutingLayer_T* layer, RouteAddr_T* dest, RouteAddr_T* relay);
int helperRemoveNeighbor(RoutingLayer_T* layer, ChannelAddr_T* address);
int helperUpdateRoute(RoutingLayer_T* layer);
int helperUpdateNeighbor(RoutingLayer_T* layer);

#endif //MOARSTACK_MOARROUTINGTABLESHANDLER_H
