//
// Created by kryvashek on 20.08.16.
//

#include <string.h>			// snprintf(), memset(), ...
#include <time.h>			// localtime(), strftime(), ...
#include <errno.h>			// errno
#include <stdlib.h>			// malloc(), free()

#include <moarLogger.h>
#include <moarTime.h>		// timeGetCurrent()
#include <funcResults.h>

static char	*moarErrorMessages[LOG_MOAR_ERRS_COUNT] = {
	"FUNC_RESULT_SUCCESS",
	"FUNC_RESULT_FAILED",
	"FUNC_RESULT_FAILED_ARGUMENT",
	"FUNC_RESULT_FAILED_IO",
	"FUNC_RESULT_FAILED_MEM_ALLOCATION"
};

// opens log file with specified filepath; returns handler on success, value <= 0 otherwise
int LogOpen( LogFilepath_T logFile, LogLevel_T logLevel, LogHandle_T * handle ) {
	if( NULL == logFile || NULL == handle || 0 == strlen( logFile ) )
		return FUNC_RESULT_FAILED_ARGUMENT;

	*handle = malloc( sizeof( LogDescriptor_T ) );

	if( NULL == *handle )
		return FUNC_RESULT_FAILED_MEM_ALLOCATION;

	( *handle )->FileHandle = fopen( logFile, "a" );

	if( NULL == ( *handle )->FileHandle ) {
		free( *handle );
		*handle = NULL;
		return FUNC_RESULT_FAILED_IO;
	}

	( *handle )->MinLogLevel = logLevel;

	return FUNC_RESULT_SUCCESS;
}

int logMoment( LogHandle_T handle ) {
	int			result;
	time_t		seconds;
	moarTime_T	moment;
	size_t		length;

	memset( handle->MomentBuffer, 0, LOG_TIMESTAMP_SIZE );
	moment = timeGetCurrent();
	seconds = ( time_t )( moment / 1000 );
	strftime( handle->MomentBuffer, LOG_TIMESTAMP_SIZE, "%F %T", localtime( &seconds ) );
	length = strlen( handle->MomentBuffer );
	result = snprintf( handle->MomentBuffer + length, LOG_TIMESTAMP_SIZE - length - 1, ".%03u", ( unsigned )( moment % 1000 ) );

	if( 0 > result )
		return FUNC_RESULT_FAILED;

	result = fprintf( handle->FileHandle, "%s : ", handle->MomentBuffer );
	fflush( handle->FileHandle );

	return ( 0 > result ? FUNC_RESULT_FAILED_IO : FUNC_RESULT_SUCCESS );
}

char * logReformat( const char * startFormat ) {
	char	* finishFormat,
			* replace;
	size_t	length;

	if( NULL == startFormat )
		return NULL;

	length = strlen( startFormat );

	if( 0 == length )
		return NULL;

	finishFormat = malloc( length + 1 );

	if( NULL == finishFormat )
		return NULL;

	memcpy( finishFormat, startFormat, length );
	finishFormat[ length ] = 0;

	if( '\n' == finishFormat[ length - 1 ] )
		finishFormat[ length - 1 ] = 0;

	replace = strchr( finishFormat, '\n' );

	while( NULL != replace ) {
		*replace = ' ';
		replace = strchr( replace + 1, '\n' );
	}

	return finishFormat;
}

// writes some message to the log file specified by handle, adding time of writing
int LogWrite( LogHandle_T handle, LogLevel_T logLevel, const char * format, ... ) {
	va_list	args;
	int		result;
	char	* lineFormat;

	if( NULL == handle || NULL == format || NULL == handle->FileHandle )
		return FUNC_RESULT_FAILED_ARGUMENT;

	if( logLevel < handle->MinLogLevel )
		return FUNC_RESULT_SUCCESS;

	lineFormat = logReformat( format );

	if( NULL == lineFormat )
		return FUNC_RESULT_FAILED;

	result = logMoment( handle );

	if( FUNC_RESULT_SUCCESS != result )
		return result;

	va_start( args, format );
	result = vfprintf( handle->FileHandle, lineFormat, args );
	va_end( args );
	fprintf( handle->FileHandle, "\n" );
	fflush( handle->FileHandle );
	free( lineFormat );

	return ( 0 > result ? FUNC_RESULT_FAILED_IO : FUNC_RESULT_SUCCESS );
}

// writes system error message to the log file specified by handle, adding time of writing
int LogErrSystem( LogHandle_T handle, LogLevel_T logLevel, const char * message ) {
	const int	errorValue = errno;
	int			result;

	if( NULL == handle || NULL == handle->FileHandle )
		return FUNC_RESULT_FAILED_ARGUMENT;

	if( logLevel < handle->MinLogLevel )
		return FUNC_RESULT_SUCCESS;

	result = logMoment( handle );

	if( FUNC_RESULT_SUCCESS != result )
		return result;

	if( NULL == message )
		result = fprintf( handle->FileHandle, "system error %d (%s)\n", errorValue, strerror( errorValue ) );
	else {
		char	* lineMessage = logReformat( message );

		if( NULL == lineMessage )
			return FUNC_RESULT_FAILED;

		result = fprintf( handle->FileHandle, "system error %d (%s) : %s\n", errorValue, strerror( errorValue ),
						  lineMessage );
		free( lineMessage );
	}

	fprintf( handle->FileHandle, "\n" );
	fflush( handle->FileHandle );

	return ( 0 > result ? FUNC_RESULT_FAILED_IO : FUNC_RESULT_SUCCESS );
}

// writes moar error message to the log file specified by handle, adding time of writing
int LogErrMoar( LogHandle_T handle, LogLevel_T logLevel, int returnResult, const char * message ) {
	int	result;

	if( NULL == handle || NULL == handle->FileHandle )
		return FUNC_RESULT_FAILED_ARGUMENT;

	if( logLevel < handle->MinLogLevel )
		return FUNC_RESULT_SUCCESS;

	result = logMoment( handle );

	if( FUNC_RESULT_SUCCESS != result )
		return result;

	if( NULL == message )
		result = fprintf( handle->FileHandle, "moar error %d (%s)\n", returnResult, moarErrorMessages[ returnResult ] );
	else {
		char	* lineMessage = logReformat( message );

		if( NULL == lineMessage )
			return FUNC_RESULT_FAILED;

		result = fprintf( handle->FileHandle, "moar error %d (%s) : %s\n", returnResult,
						  moarErrorMessages[ returnResult ], lineMessage );
		free( lineMessage );
	}

	fprintf( handle->FileHandle, "\n" );
	fflush( handle->FileHandle );

	return ( 0 > result ? FUNC_RESULT_FAILED_IO : FUNC_RESULT_SUCCESS );
}

// closes log file specified by given handle
int LogClose( LogHandle_T * handle ) {
	int	result;

	if( NULL == handle || NULL == *handle || NULL == ( *handle )->FileHandle )
		return FUNC_RESULT_FAILED_ARGUMENT;

	fflush( ( *handle )->FileHandle );
	result = fclose( ( *handle )->FileHandle );

	if( 0 != result )
		return FUNC_RESULT_FAILED_IO;

	( *handle )->FileHandle = NULL;
	free( *handle );
	*handle = NULL;

	return FUNC_RESULT_SUCCESS;
}
