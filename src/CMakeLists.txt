# Copyright 2014 Jetperch LLC.  See LICENSE.txt
# libusbk_examples/src

add_executable(altsetting
    altsetting_main.cpp
    log.c
    log.h
    log_api.h
    portable_string.h
)
target_link_libraries(altsetting ${LIBUSBK_LIBS})
install(TARGETS altsetting DESTINATION "bin")
