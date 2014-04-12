/* Copyright 2013 Jetperch LLC.  All rights reserved. */

#include "log.h"
#include "portable_string.h"
#include <Windows.h>

#define LOG_LEVEL LOG_LEVEL_NOTICE
static int console_level_ = LOG_LEVEL_NOTICE;
static int log_level_ = LOG_LEVEL_INFO;
static FILE * log_file_ = NULL;


/* Trivial logging support. */

int timeString(char * str, size_t sz) {
	SYSTEMTIME t;
	GetSystemTime(&t);
	return c99_snprintf(str, sz, "%04d%02d%02d_%02d%02d%02d.%03d",
		t.wYear, t.wMonth, t.wDay, t.wHour, t.wMinute, t.wSecond, t.wMilliseconds);
}

void console_print(char const * msg) {
	char timestr[256];
	timeString(timestr, sizeof(timestr));
	printf("%s: %s\n", timestr, msg);
	fflush(stdout);
}

void log_callback_default(void * cookie, int level, const char * msg) {
	char timestr[256];
	timeString(timestr, sizeof(timestr));
	if ((NULL != log_file_) && (level <= log_level_)) {
		fprintf(log_file_, "%s: %s\n", timestr, msg);
		fflush(log_file_);
	}

	if (level <= console_level_) {
		console_print(msg);
	}
}

static log_callback log_callback_ = log_callback_default;
static void * log_cookie_ = 0;

 void logf_event(int level, const char * format, ...) {
    if (level > log_level_ || !log_callback_) {
        return;
    }
    char str[8192];
    va_list ap;
    va_start(ap, format);
    c99_vsnprintf(str, sizeof(str), format, ap);
    va_end(ap);
    log_callback_(log_cookie_, level, str);
}

void logs_event(int level, const char * msg) {
    if (level > log_level_ || !log_callback_) {
        return;
    }
    log_callback_(log_cookie_, level, msg);
}

void logc_event(int level, char c) {
    if (level > log_level_ || !log_callback_) {
        return;
    }
    char str[] = {0, 0};
    str[0] = c;
    log_callback_(log_cookie_, level, str);
}

int log_filename(const char * filename) {
	if (NULL != log_file_) {
		fclose(log_file_);
		log_file_ = NULL;
	}
	log_file_ = fopen(filename, "wt");
	return (NULL != log_file_);
}
