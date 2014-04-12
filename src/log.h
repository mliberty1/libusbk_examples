/* Copyright 2013 Jetperch LLC.  See LICENSE.txt */

/*!
 * \file
 *
 * \brief Trivial logging support.
 */

#ifndef LOG_H_
#define LOG_H_

#ifdef __cplusplus
extern "C" {
#endif

void logf_event(int level, const char * format, ...);
void logs_event(int level, const char * msg);
void logc_event(int level, char c);
int log_filename(const char * filename);

#define LOG_GLOBAL_LEVEL LOG_LEVEL_ALL
#define LOGF(level, format, ...) if ((level <= LOG_GLOBAL_LEVEL) && (level <= LOG_LEVEL)) { logf_event(level, format, __VA_ARGS__); }
#define LOGS(level, msg) if ((level <= LOG_GLOBAL_LEVEL) && (level <= LOG_LEVEL)) { logs_event(level, msg); }
#define LOGC(level, c)   if ((level <= LOG_GLOBAL_LEVEL) && (level <= LOG_LEVEL)) { logc_event(level, c); }
#include "log_api.h"

#ifdef __cplusplus
}
#endif

#endif /* LOG_H_ */
