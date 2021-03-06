//
// Created by kryvashek on 07.07.16.
// for public notations

#ifndef MOARSTACK_MOARSERVICE_H_H
#define MOARSTACK_MOARSERVICE_H_H

#include <moarApiCommon.h>
#include <stdint.h>

typedef uint16_t ServiceDataSize_T;
#pragma pack(push, 1)
typedef struct{
	AppId_T LocalAppId;
	AppId_T RemoteAppId;
	ServiceDataSize_T PayloadSize;
}ServiceLayerHeader_T;
#pragma pack(pop)

#endif //MOARSTACK_MOARSERVICE_H_H
