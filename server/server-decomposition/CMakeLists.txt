# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(server)

set(SOURCE_FILES
    src/main.c
    src/led.c
    src/button.c
    src/coap.c
    src/crypto.c
    src/json.c
)
target_sources(app PRIVATE ${SOURCE_FILES})
