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

# @file xia_configure_target.cmake
# @brief Defines a function used to configure targets on Windows and Linux

function(xia_configure_target)
    set(options USE_PLX FORCE_DEBUG CONFIG_OBJ LEGACY)
    set(oneValueArgs TARGET CXX_STD)
    set(multiValueArgs COMPILE_DEFS COMPILE_OPTS WIN_COMPILE_OPTIONS LINUX_COMPILE_OPTIONS LIBS LINUX_LIBS WIN_LIBS)
    cmake_parse_arguments(XIA_CT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if (NOT XIA_CT_CXX_STD)
        set(XIA_CT_CXX_STD 14)
    endif ()

    if (${CMAKE_VERSION} VERSION_GREATER "3.0.2")
        set_target_properties(${XIA_CT_TARGET}
                PROPERTIES
                CXX_STANDARD ${XIA_CT_CXX_STD}
                CXX_STANDARD_REQUIRED YES
                CXX_EXTENSIONS NO
                )
    else ()
        if (NOT LEGACY)
            target_compile_options(${XIA_CT_TARGET} PRIVATE -std=c++${XIA_CT_CXX_STD})
        endif ()
    endif ()

    if (${XIA_CT_USE_PLX})
        target_include_directories(${XIA_CT_TARGET} PUBLIC ${PLX_INCLUDE_DIR})
        if (${CMAKE_VERSION} VERSION_GREATER "3.12")
            target_link_directories(${XIA_CT_TARGET} PUBLIC ${PLX_LIBRARY_DIR})
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            target_link_libraries(${XIA_CT_TARGET} PUBLIC ${PLX_STATIC_LIB})
        endif ()
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_LITTLE_ENDIAN)
    endif ()

    target_compile_definitions(${XIA_CT_TARGET} PUBLIC PCI_CODE ${XIA_CT_COMPILE_DEFS})
    target_compile_options(${XIA_CT_TARGET} PUBLIC ${XIA_CT_COMPILE_OPTS})

    if (NOT ${XIA_CT_CONFIG_OBJ})
        target_link_libraries(${XIA_CT_TARGET} PUBLIC ${XIA_CT_LIBS})
    endif ()

    if ((${CMAKE_BUILD_TYPE} MATCHES "Debug") OR ${XIA_CT_FORCE_DEBUG})
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC PRINT_DEBUG_MSG)
    endif ()

    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        if (${XIA_CT_USE_PLX})
            target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_LINUX)
            if (NOT ${XIA_CT_CONFIG_OBJ})
                target_link_libraries(${XIA_CT_TARGET} PUBLIC pthread)
            endif ()
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            target_link_libraries(${XIA_CT_TARGET} PUBLIC ${XIA_CT_LINUX_LIBS})
        endif ()
        if (${CMAKE_BUILD_TYPE} MATCHES "Debug")
            target_compile_options(${XIA_CT_TARGET} PUBLIC -Wall -Wextra)
        endif ()
        target_compile_options(${XIA_CT_TARGET} PUBLIC ${XIA_CT_LINUX_COMPILE_OPTIONS})
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        if (${XIA_CT_USE_PLX})
            target_compile_definitions(${XIA_CT_TARGET} PUBLIC PLX_WIN)
        endif ()
        if (NOT ${XIA_CT_CONFIG_OBJ})
            if (NOT ${XIA_CT_LEGACY})
                target_link_libraries(${XIA_CT_TARGET} PUBLIC ${XIA_CT_WIN_LIBS})
            else ()
                target_link_libraries(${XIA_CT_TARGET} PUBLIC winmm ${XIA_CT_WIN_LIBS})
            endif ()
        endif ()
        target_compile_options(${XIA_CT_TARGET} PUBLIC ${XIA_CT_WIN_COMPILE_OPTIONS})
        target_compile_definitions(${XIA_CT_TARGET} PUBLIC _CRT_SECURE_NO_WARNINGS)
    endif ()
endfunction()
