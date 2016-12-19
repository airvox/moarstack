#ifndef __MOAR_API_H__
#define __MOAR_API_H__

#include "moarApiCommon.h"

__BEGIN_DECLS

extern int moarSocket(MoarDesc_T *fd);

extern int moarBind(MoarDesc_T fd, const AppId_T *appId);

extern ssize_t moarRecvFrom(MoarDesc_T fd, void *msg, size_t len, RouteAddr_T *routeAddr, AppId_T  *appId);

extern ssize_t moarSendTo(MoarDesc_T fd, const void *msg, size_t len, const RouteAddr_T *routeAddr, const AppId_T *appId, MessageId_T *msgId);

extern MessageState_T moarMsgState(MoarDesc_T fd, const MessageId_T *msgId);

__END_DECLS

#endif
