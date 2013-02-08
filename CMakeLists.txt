cmake_minimum_required(VERSION 2.8)
project(thermostat)
set(CMAKE_BUILD_TYPE Debug)

option(BUILD_HIDAPI_SHARED "Build with shared version of HIDAPI" ON)

find_path(HIDAPI_HEADER_DIR hidapi.h
  PATH_SUFFIXES hidapi
  DOC "The location of HIDAPI's header file"
)

if (BUILD_HIDAPI_SHARED)
  find_library(HIDAPI_LIB NAMES hidapi-hidraw hidapi-libusb
    PATH_SUFFIXES linux libusb mac
    DOC "The location of the HIDAPI shared library file"
  )
  set(HIDAPI_STATIC_OBJECT)
  set(HIDAPI_LINK_LIBS ${HIDAPI_LIB})
else()
  set(HIDAPI_LIB)
  find_file(HIDAPI_OBJECT NAMES hid.o hid-libusb.o
    PATHS ../hidapi ../hidapi.git
    PATH_SUFFIXES linux libusb mac
    DOC "The location of the HIDAPI static object file"
  )
  set(HIDAPI_STATIC_OBJECT ${HIDAPI_OBJECT})
  find_package(PkgConfig REQUIRED)
  if (HIDAPI_OBJECT MATCHES \(-libusb|/libusb/hid\)\\.o\$)
    pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
    set(HIDAPI_LINK_LIBS ${LIBUSB_LIBRARIES} rt pthread)
  else()
    pkg_check_modules(LIBUDEV REQUIRED libudev)
    set(HIDAPI_LINK_LIBS ${LIBUDEV_LIBRARIES})
  endif()
endif()

include_directories(${HIDAPI_HEADER_DIR})

add_executable(
  thermostat
  main.c
)

target_link_libraries(
        thermostat
        tempered
	hidapi-libusb
	rrd
)
