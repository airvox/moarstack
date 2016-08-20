//
// Created by kryvashek on 20.08.16.
//

#ifndef MOARSTACK_MOARLOGGER_H
#define MOARSTACK_MOARLOGGER_H

#include <stdio.h>
#include <stdarg.h>

#include <funcResults.h>

#define LOG_FILEPATH_SIZE	108 // just chosen to be the same as length of SOCKET_FILEPATH_SIZE
#define LOG_TIMESTAMP_SIZE	30	// just copied from MockIT
#define LOG_MOAR_ERRS_COUNT	(FUNC_RESULT_SUCCESS-FUNC_RESULT_FAILED_MEM_ALLOCATION+1)

typedef char	LogFilepath_T[ LOG_FILEPATH_SIZE ];
typedef char	LogMoment_T[ LOG_TIMESTAMP_SIZE ];
typedef FILE	* LogHandle_T;

#ifdef __cplusplus
extern "C" {
#endif

// opens log file with specified filepath; returns handler on success, value <= 0 otherwise
extern int LogOpen( LogFilepath_T logFile, LogHandle_T * handle );

// writes some message to the log file specified by handle, adding time of writing
extern int LogWrite( LogHandle_T handle, const char * format, ... );

// writes system error message to the log file specified by handle, adding time of writing
extern int LogErrSystem( LogHandle_T handle, const char * message );

// writes moar error message to the log file specified by handle, adding time of writing
extern int LogErrMoar( LogHandle_T handle, int returnResult, const char * message );

// closes log file specified by given handle
extern int LogClose( LogHandle_T * handle );

#ifdef __cplusplus
}
#endif


#endif //MOARSTACK_MOARLOGGER_H
