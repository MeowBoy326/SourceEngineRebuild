set(PROJECT_NAME choreoobjects)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "../../game/shared/choreoactor.h"
        "../../game/shared/choreochannel.h"
        "../../game/shared/choreoevent.h"
        "../../game/shared/choreoscene.h"
        "../../game/shared/ichoreoeventcallback.h"
        "../../game/shared/sceneimage.h"
        )
source_group("Header Files" FILES ${Header_Files})

set(Public_Header_Files
        "../../public/mathlib/mathlib.h"
        "../../public/mathlib/vector.h"
        "../../public/mathlib/vector2d.h"
        )
source_group("Public Header Files" FILES ${Public_Header_Files})

set(Source_Files
        "../../common/debug_lib_check.cpp"
        "../../game/shared/choreoactor.cpp"
        "../../game/shared/choreochannel.cpp"
        "../../game/shared/choreoevent.cpp"
        "../../game/shared/choreoscene.cpp"
        "../../game/shared/sceneimage.cpp"
        )
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
        ${Header_Files}
        ${Public_Header_Files}
        ${Source_Files}
        )

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
################################################################################
# Target name
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        TARGET_NAME_DEBUG "choreoobjects"
        TARGET_NAME_RELEASE "choreoobjects"
        )
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG "${MYROOTDIR}/../../game/bin"
        OUTPUT_DIRECTORY_RELEASE "${MYROOTDIR}/../../game/bin"
        )
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
        MultiThreadedDebug
        >
        $<$<CONFIG:Release>:
        MultiThreaded
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
        )
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJECT_NAME} PRIVATE
        "${MYROOTDIR}/common;"
        "${MYROOTDIR}/public;"
        "${MYROOTDIR}/../thirdparty/SDL2;"
        "${MYROOTDIR}/public/tier0;"
        "${MYROOTDIR}/public/tier1;"
        "${MYROOTDIR}/game/shared;"
        "${MYROOTDIR}/utils/common"
        )

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
        "DISABLE_ETW;"
        "_DEBUG;"
        "DEBUG;"
        "_SCL_SECURE_NO_WARNINGS"
        ">"
        "$<$<CONFIG:Release>:"
        "NDEBUG;"
        "_ALLOW_RUNTIME_LIBRARY_MISMATCH;"
        "_ALLOW_ITERATOR_DEBUG_LEVEL_MISMATCH;"
        "_ALLOW_MSC_VER_MISMATCH"
        ">"
        "VPC;"
        "RAD_TELEMETRY_DISABLED;"
        "LIBNAME=choreoobjects;"
        "BINK_VIDEO;"
        "AVI_VIDEO;"
        "WMV_VIDEO;"
        "DEV_BUILD;"
        "FRAME_POINTER_OMISSION_DISABLED;"
        "SOURCE1=1;"
        "FUCK_STEAM;"
        "_MBCS"
        )

################################################################################
# Compile and link options
################################################################################
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
            /Od;
            /ZI
            >
            $<$<CONFIG:Release>:
            /O2;
            /Ob2;
            /Oi;
            /Gy;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /Ot;
            /Zo;
            /Oy-
            >
            /MP;
            /permissive;
            /GF;
            /W4;
            /WX-;
            /TP;
            /fp:fast;
            /GR;
            /wd4316;
            /wd4577;
            /wd4091;
            /Gw;
            /Zc:inline;
            /Zc:threadSafeInit-;
            /Wv:18;
            /GS;
            /Zc:wchar_t;
            /Zc:forScope;

            )

endif ()

