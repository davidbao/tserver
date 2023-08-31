#*******************************************************************************
#  Copyright (c) 2022 com.
#
#  All rights reserved. This program and the accompanying materials
#  are made available under the terms of the EaseSuite Public License v1.0
#  and EaseSuite Distribution License v1.0 which accompany this distribution.
#
#  Contributors:
#     Baowei - initial version
#*******************************************************************************

# Note: on OS X you should install XCode and the associated command-line tools
if (TSERVER_ENABLE_TEST)
    set(CMAKE_CXX_STANDARD ${TSERVER_CXX_STANDARD})

    # include settings
    include_directories(${EXTERNALS_PATH}/install/${OS_NAME}/include)
    include_directories(${EXTERNALS_PATH}/install/${OS_NAME}/include/libxml2)
    include_directories(${EXTERNALS_PATH}/install/${OS_NAME}/include/libiconv)
    include_directories(${EXTERNALS_PATH}/include)
    include_directories(${CMAKE_SOURCE_DIR}/inc)
    include_directories(${CMAKE_SOURCE_DIR}/inc/common)

    # link settings
    # externals path
    if (WIN_OS)
        link_directories(${EXTERNALS_PATH}/install/${OS_NAME}/lib/${CMAKE_VS_PLATFORM_NAME}/${CMAKE_BUILD_TYPE})
        link_directories(${EXTERNALS_PATH}/lib/${OS_NAME}/${CMAKE_VS_PLATFORM_NAME}/${CMAKE_BUILD_TYPE})
    else ()
        link_directories(${EXTERNALS_PATH}/install/${OS_NAME}/lib)
        link_directories(${EXTERNALS_PATH}/lib/${OS_NAME})
    endif ()

    # common path
    set(COMMON_LIB ${CMAKE_SOURCE_DIR}/lib/common)
    if (WIN_OS)
        link_directories(${COMMON_LIB}/${OS_NAME}/${CMAKE_VS_PLATFORM_NAME}/${CMAKE_BUILD_TYPE})
    else ()
        link_directories(${COMMON_LIB}/${OS_NAME}/${CMAKE_BUILD_TYPE})
    endif ()

    # inner path
    link_directories(${CMAKE_SOURCE_DIR}/lib)

    # lib settings
    if (WIN32 OR MSVC)
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
    elseif (UNIX)
        # Only search static lib.
        set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    endif ()
    set(EXTERNALS_LIB
            mysqlclient${CMAKE_FIND_LIBRARY_SUFFIXES}
            sqlite3${CMAKE_FIND_LIBRARY_SUFFIXES}
            event_openssl${CMAKE_FIND_LIBRARY_SUFFIXES}
            event${CMAKE_FIND_LIBRARY_SUFFIXES}
            curl${CMAKE_FIND_LIBRARY_SUFFIXES}
            zip${CMAKE_FIND_LIBRARY_SUFFIXES}
            yaml-cpp${CMAKE_FIND_LIBRARY_SUFFIXES}
            bz2${CMAKE_FIND_LIBRARY_SUFFIXES}
            xml2${CMAKE_FIND_LIBRARY_SUFFIXES}
            z${CMAKE_FIND_LIBRARY_SUFFIXES}
            ssl${CMAKE_FIND_LIBRARY_SUFFIXES}
            crypto${CMAKE_FIND_LIBRARY_SUFFIXES}
            json${CMAKE_FIND_LIBRARY_SUFFIXES}
            paho-mqtt3a-static${CMAKE_FIND_LIBRARY_SUFFIXES}
            paho-mqtt3as-static${CMAKE_FIND_LIBRARY_SUFFIXES}
            paho-mqtt3c-static${CMAKE_FIND_LIBRARY_SUFFIXES}
            paho-mqtt3cs-static${CMAKE_FIND_LIBRARY_SUFFIXES}
            )

    if (NOT WIN_OS)
        set(EXTERNALS_LIB
                event_pthreads${CMAKE_FIND_LIBRARY_SUFFIXES}
                ${EXTERNALS_LIB}
                )
    endif ()

    if (TSERVER_STATIC_STDLIB)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} stdc++${CMAKE_FIND_LIBRARY_SUFFIXES})
    endif ()

    if (MAC_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} iconv lzma)
    elseif (LINUX_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} uuid pthread dl rt lzma)
    elseif (ARM_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} uuid${CMAKE_FIND_LIBRARY_SUFFIXES} pthread dl lzma)
    elseif (ANDROID_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} iconv uuid log)
    elseif (WIN_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB} iconv Shlwapi ws2_32 Rpcrt4 shell32 advapi32 Iphlpapi Crypt32 Wldap32)
    elseif (WEB_OS)
        set(EXTERNALS_LIB ${EXTERNALS_LIB})
    endif ()

    if (TSERVER_BUILD_KINGBASE)
        if (WIN_OS)
            set(EXTERNALS_LIB kci ${EXTERNALS_LIB})
        else ()
            set(EXTERNALS_LIB kci crypto ${EXTERNALS_LIB})
        endif ()
    endif ()

    set(COMMON_LIB common${CMAKE_FIND_LIBRARY_SUFFIXES} ${EXTERNALS_LIB})
    set(TSERVER_LIB ${COMMON_LIB})
    #    message(STATUS "TSERVER_LIB: " ${TSERVER_LIB})

    # application settings
    if (MAC_OS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework Foundation -framework SystemConfiguration")
    endif ()

    set(TSERVER_SRC ../src/Style.cpp)
    set(TEST_SRC StyleTest.cpp)
    set(TSERVER_SRC ${TSERVER_SRC} ../src/tasks/Schedule.cpp)
    set(TEST_SRC ${TEST_SRC} ScheduleTest.cpp)

    foreach (item ${TEST_SRC})
        string(REGEX MATCH "[0-9A-Za-z_]*" app ${item})
        add_executable(${app} ${TSERVER_SRC} ${item})
        target_link_libraries(${app} ${TSERVER_LIB})
        add_test(NAME ${app} COMMAND ${app})
    endforeach ()
endif ()
