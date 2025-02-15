set(PROJECT_NAME shaderlib)

################################################################################
# Source groups
################################################################################
set(Header_Files
        "../../public/appframework/IAppSystem.h"
        "../../public/icvar.h"
        "../../public/materialsystem/imaterial.h"
        "../../public/materialsystem/imaterialsystem.h"
        "../../public/materialsystem/imaterialsystemhardwareconfig.h"
        "../../public/materialsystem/imaterialvar.h"
        "../../public/materialsystem/imesh.h"
        "../../public/materialsystem/IShader.h"
        "../../public/materialsystem/ishaderapi.h"
        "../../public/materialsystem/itexture.h"
        "../../public/materialsystem/materialsystem_config.h"
        "../../public/mathlib/mathlib.h"
        "../../public/mathlib/vector.h"
        "../../public/mathlib/vector2d.h"
        "../../public/mathlib/vector4d.h"
        "../../public/mathlib/vmatrix.h"
        "../../public/mathlib/vplane.h"
        "../../public/shaderlib/BaseShader.h"
        "../../public/shaderlib/cshader.h"
        "../../public/shaderlib/ShaderDLL.h"
        "../../public/string_t.h"
        "../../public/tier0/basetypes.h"
        "../../public/tier0/commonmacros.h"
        "../../public/tier0/dbg.h"
        "../../public/tier0/fasttimer.h"
        "../../public/tier0/icommandline.h"
        "../../public/tier0/memdbgoff.h"
        "../../public/tier0/memdbgon.h"
        "../../public/tier0/platform.h"
        "../../public/tier0/protected_things.h"
        "../../public/tier1/strtools.h"
        "../../public/tier1/utlmemory.h"
        "../../public/tier1/utlvector.h"
        "../../public/vstdlib/vstdlib.h"
        "../IShaderSystem.h"
        "shaderDLL_Global.h"
        "shaderlib_cvar.h"
        )
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
        "../../common/debug_lib_check.cpp"
        "BaseShader.cpp"
        "ShaderDLL.cpp"
        "shaderlib_cvar.cpp"
        )
source_group("Source Files" FILES ${Source_Files})



set(ALL_FILES
        ${Header_Files}
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
        TARGET_NAME_DEBUG "shaderlib"
        TARGET_NAME_RELEASE "shaderlib"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/.."
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
        "LIBNAME=shaderlib;"
        "BINK_VIDEO;"
        "AVI_VIDEO;"
        "WMV_VIDEO;"
        "DEV_BUILD;"
        "FRAME_POINTER_OMISSION_DISABLED;"
        "FAST_MATERIALVAR_ACCESS;"
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

