project(Joschy)

cmake_minimum_required(VERSION 2.6)


# vars
set(BUILD_BLIP true)
set(BUILD_YOUTUBE true)
set(BUILD_QNETWORKLAYER true)
set(BUILD_KNETWORKLAYER false)

if (WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-enable-auto-image-base -Wl,-enable-auto-import")
endif (WIN32)


# find Qt4
find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})
 
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(FeatureSummary)

find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(KF5 REQUIRED COMPONENTS KDELibs4Support I18n)

# definitions
add_definitions(${QT_DEFINITIONS} -DJOSCHY_LIBRARY)

# build with core
set(BUILD_WITH_JOSCHYCORE true)

# core
add_subdirectory(joschycore)

set(JOSCHYCORE_PLUGIN_INSTALL_DIR ${JOSCHY_PLUGIN_INSTALL_DIR})
set(JOSCHYCORE_LIBRARIES joschycore)
set(JOSCHYCORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/joschycore/)

if (BUILD_KNETWORKLAYER)
    find_package(KF5)
endif (BUILD_KNETWORKLAYER)


if (BUILD_QNETWORKLAYER)
    add_subdirectory(QNetworkLayer)
endif (BUILD_QNETWORKLAYER)

if (KF5_FOUND)
    message(STATUS "KNetworkLayer needs an update...")
    #add_subdirectory(KNetworkLayer)
endif(KF5_FOUND)

add_subdirectory(Provider)

