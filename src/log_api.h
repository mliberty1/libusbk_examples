/* Copyright 2013 Jetperch LLC.  All rights reserved. */

/*!
 * \file
 *
 * \brief Trivial logging support.
 */

#ifndef LOG_API_H_
#define LOG_API_H_


/**
* \brief Function called when a logging is requested.
*
* \param cookie The data passed to function when the callback was specified.
* \param level The logging level (log_level_e).
* \param msg The log message.  This pointer is only valid for the duration of
*      the callback.
*/
typedef void(*log_callback)(void * cookie, int level, const char * msg);



/**
 * \brief The logging levels available to the system.
 */
enum log_level_e {
    /** Logging functionality is disabled. */
    LOG_LEVEL_OFF         = -1,
    /** A critical error which prevents further functions. */
    LOG_LEVEL_CRITICAL    = 2,
    /** An error which prevents the current operation from completing or
     *  will adversely effect future functionality. */
    LOG_LEVEL_ERROR       = 3,
    /** A warning which may adversely affect the current operation or future
     *  operations. */
    LOG_LEVEL_WARN        = 4,
    /** A notification for interesting events. */
    LOG_LEVEL_NOTICE      = 5,
    /** An informative message. */
    LOG_LEVEL_INFO        = 6,
    /** Detailed messages for the software developer. */
    LOG_LEVEL_DEBUG       = 7,
    /** All logging functionality is enabled. */
    LOG_LEVEL_ALL         = 10,
};


// Define LOG_GLOBAL_LEVEL before including this file.
#ifndef LOG_GLOBAL_LEVEL
/** The maximum logging level permitted for all logs. */
#define LOG_GLOBAL_LEVEL LOG_LEVEL_ALL
#endif

// Define LOGF before including this file.
#ifndef LOGF
/*!
 * \brief Macro to log a printf-compatible formatted string.
 *
 * \param level The log_level_e.
 * \param format The printf-compatible formatting string.
 * \param ... The arguments to the formatting string.
 */
#define LOGF(level, format, ...)
#endif

// Define LOGS before including this file.
#ifndef LOGS
/*!
 * \brief Macro to log a string.
 *
 * \param level The log_level_e.
 * \param msg The string to log.
 */
#define LOGS(level, msg)
#endif

// Define LOGC before including this file.
#ifndef LOGC
/*!
 * \brief Macro to log a character.
 *
 * \param level The log_level_e.
 * \param c The character to log.
 */
#define LOGC(level, c)
#endif


/** Log a critical failure using printf-style arguments. */
#define LOGF_CRITICAL(format, ...)  LOGF(LOG_LEVEL_CRITICAL, format, __VA_ARGS__)
/** Log an error using printf-style arguments.  Same as LOGF_ERR. */
#define LOGF_ERR(format, ...)       LOGF(LOG_LEVEL_ERROR,    format, __VA_ARGS__)
/** Log an error using printf-style arguments. */
#define LOGF_ERROR(format, ...)     LOGF(LOG_LEVEL_ERROR,    format, __VA_ARGS__)
/** Log an warning using printf-style arguments. */
#define LOGF_WARN(format, ...)      LOGF(LOG_LEVEL_WARN,     format, __VA_ARGS__)
/** Log an warning using printf-style arguments.  Same as LOGF_WARN. */
#define LOGF_WARNING(format, ...)   LOGF(LOG_LEVEL_WARN,     format, __VA_ARGS__)
/** Log a notice using printf-style arguments. */
#define LOGF_NOTICE(format, ...)    LOGF(LOG_LEVEL_NOTICE,   format, __VA_ARGS__)
/** Log an informative message using printf-style arguments. */
#define LOGF_INFO(format, ...)      LOGF(LOG_LEVEL_INFO,     format, __VA_ARGS__)
/** Log a detailed debug message using printf-style arguments.  Same as LOGF_DEBUG. */
#define LOGF_DBG(format, ...)       LOGF(LOG_LEVEL_DEBUG,    format, __VA_ARGS__)
/** Log a detailed debug message using printf-style arguments. */
#define LOGF_DEBUG(format, ...)     LOGF(LOG_LEVEL_DEBUG,    format, __VA_ARGS__)

/** Log a critical failure string. */
#define LOGS_CRITICAL(msg)  LOGS(LOG_LEVEL_CRITICAL, msg)
/** Log an error string.  Same as LOGS_ERR. */
#define LOGS_ERR(msg)       LOGS(LOG_LEVEL_ERROR,    msg)
/** Log an error string. */
#define LOGS_ERROR(msg)     LOGS(LOG_LEVEL_ERROR,    msg)
/** Log an warning string. */
#define LOGS_WARN(msg)      LOGS(LOG_LEVEL_WARN,     msg)
/** Log an warning string.  Same as LOGS_WARN. */
#define LOGS_WARNING(msg)   LOGS(LOG_LEVEL_WARN,     msg)
/** Log a notice string. */
#define LOGS_NOTICE(msg)    LOGS(LOG_LEVEL_NOTICE,   msg)
/** Log an informative message string. */
#define LOGS_INFO(msg)      LOGS(LOG_LEVEL_INFO,     msg)
/** Log a detailed debug message string.  Same as LOGS_DEBUG. */
#define LOGS_DBG(msg)       LOGS(LOG_LEVEL_DEBUG,    msg)
/** Log a detailed debug message string. */
#define LOGS_DEBUG(msg)     LOGS(LOG_LEVEL_DEBUG,    msg)

/** Log a critical failure character. */
#define LOGC_CRITICAL(c)    LOGC(LOG_LEVEL_CRITICAL, c)
/** Log an error character.  Same as LOGC_ERR. */
#define LOGC_ERR(c)         LOGC(LOG_LEVEL_ERROR,    c)
/** Log an error character. */
#define LOGC_ERROR(c)       LOGC(LOG_LEVEL_ERROR,    c)
/** Log a warning character. */
#define LOGC_WARN(c)        LOGC(LOG_LEVEL_WARN,     c)
/** Log a warning character.  Same as LOGC_WARN. */
#define LOGC_WARNING(c)     LOGC(LOG_LEVEL_WARN,     c)
/** Log a notice character. */
#define LOGC_NOTICE(c)      LOGC(LOG_LEVEL_NOTICE,   c)
/** Log an informative character. */
#define LOGC_INFO(c)        LOGC(LOG_LEVEL_INFO,     c)
/** Log a debug character.  Same as LOGC_DEBUG. */
#define LOGC_DBG(c)         LOGC(LOG_LEVEL_DEBUG,    c)
/** Log a debug character. */
#define LOGC_DEBUG(c)       LOGC(LOG_LEVEL_DEBUG,    c)


#endif /* LOG_API_H_ */
