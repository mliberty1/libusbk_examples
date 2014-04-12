// Copyright 2013 Jetperch LLC.  All rights reserved.

#ifndef PORTABLE_STRING_H_
#define PORTABLE_STRING_H_

// Just because of Microsoft's lazy stance on C99...

#include <stdio.h>
#include <stdarg.h>

/* http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010/8712996#8712996 */
#ifdef _MSC_VER

#define snprintf c99_snprintf

int c99_vsnprintf(char* str, size_t size, const char* format, va_list ap) {
    int count = -1;

    if (size != 0)
        count = _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    if (count == -1)
        count = _vscprintf(format, ap);

    return count;
}

int c99_snprintf(char* str, size_t size, const char* format, ...) {
	int count;
	va_list ap;

	va_start(ap, format);
	count = c99_vsnprintf(str, size, format, ap);
	va_end(ap);

	return count;
}

#else // _MSC_VER
#define c99_vsnprintf vsnprintf
#define c99_snprintf snprintf
#endif

#endif // PORTABLE_STRING_H_
