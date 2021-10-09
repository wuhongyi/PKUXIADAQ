# SPDX-License-Identifier: Apache-2.0

# Copyright 2021 XIA LLC, All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# @file FindBroadcomAPI.cmake
# @brief Find the Broadcom PCI/PCIe library
#
# This module defines the following variables:
# PLX_LIBRARY_DIR - Location of the PlxApi* files.
# PLX_INCLUDE_DIR - Location of the Plx header files.
# PLX_STATIC_LIBRARY_PATH - Full path to the PLX static library.
# PLX_STATIC_LIB - Library list necessary for compiling against the static library.
# PLX_FOUND - TRUE if we found PLX_LIBRARY_DIR, PLX_INCLUDE_DIR, PLX_STATIC_LIBRARY_PATH
#
# TODO: We will need to update this so that we find the proper libraries based on architecture.
#

find_path(PLX_LIBRARY_DIR
        NAMES PlxApi.a PlxApi.lib
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk C:/PlxApi
        PATH_SUFFIXES PlxApi/Library Linux/PlxApi/Library PlxApi/Release)

find_library(PLX_STATIC_LIBRARY_PATH
        NAMES PlxApi.a PlxApi.lib
        HINTS ${PLX_LIBRARY_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk C:/PlxApi
        PATH_SUFFIXES PlxApi/Library Linux/PlxApi/Library PlxApi/Release
        )

find_path(PLX_INCLUDE_DIR
        NAMES Plx.h PlxTypes.h PlxApi.h
        HINTS $ENV{PLX_SDK_DIR}
        PATHS /usr/local/broadcom/current /usr/src/PlxSdk
        PATH_SUFFIXES Include
        )

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(BroadcomAPI DEFAULT_MSG
        PLX_LIBRARY_DIR PLX_INCLUDE_DIR PLX_STATIC_LIBRARY_PATH)

if (BroadcomAPI_FOUND OR BROADCOMAPI_FOUND)
    mark_as_advanced(PLX_STATIC_LIBRARY_PATH PLX_LIBRARY_DIR PLX_INCLUDE_DIR)
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(PLX_STATIC_LIB -l:PlxApi.a dl m)
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        set(PLX_STATIC_LIB PlxApi.lib)
    endif ()
endif (BroadcomAPI_FOUND OR BROADCOMAPI_FOUND)
