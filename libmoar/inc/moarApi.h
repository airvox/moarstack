#ifndef __MOAR_API_H__
#define __MOAR_API_H__

#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <moarMessageId.h>
#include <moarRouting.h>


typedef int32_t MoarDesc_T;

typedef int32_t AppId_T;

typedef enum {
	MessageState_Unknown = 0,
	MessageState_Sending,
	MessageState_Sent,
	MessageState_Lost,
} MessageState_T;

__BEGIN_DECLS

extern MoarDesc_T moarSocket(void);

extern int moarBind(MoarDesc_T fd, const AppId_T *appId);

extern ssize_t moarRecvFrom(MoarDesc_T fd, void *msg, size_t len, RouteAddr_T *routeAddr, AppId_T  *appId);

extern ssize_t moarSendTo(MoarDesc_T fd, const void *msg, size_t len, const RouteAddr_T *routeAddr, const AppId_T *appId, MessageId_T *msgId);

extern MessageState_T moarMsgState(MoarDesc_T fd, const MessageId_T *msgId);

__END_DECLS

#endif
