


set(PROJECT_NAME libprotobuf)

################################################################################
# Source groups
################################################################################
set(Headers
    "../src/google/protobuf/compiler/importer.h"
    "../src/google/protobuf/compiler/parser.h"
    "../src/google/protobuf/descriptor.h"
    "../src/google/protobuf/descriptor.pb.h"
    "../src/google/protobuf/descriptor_database.h"
    "../src/google/protobuf/dynamic_message.h"
    "../src/google/protobuf/extension_set.h"
    "../src/google/protobuf/generated_message_reflection.h"
    "../src/google/protobuf/generated_message_util.h"
    "../src/google/protobuf/io/coded_stream.h"
    "../src/google/protobuf/io/coded_stream_inl.h"
    "../src/google/protobuf/io/gzip_stream.h"
    "../src/google/protobuf/io/printer.h"
    "../src/google/protobuf/io/strtod.h"
    "../src/google/protobuf/io/tokenizer.h"
    "../src/google/protobuf/io/zero_copy_stream.h"
    "../src/google/protobuf/io/zero_copy_stream_impl.h"
    "../src/google/protobuf/io/zero_copy_stream_impl_lite.h"
    "../src/google/protobuf/message.h"
    "../src/google/protobuf/message_lite.h"
    "../src/google/protobuf/reflection_ops.h"
    "../src/google/protobuf/repeated_field.h"
    "../src/google/protobuf/service.h"
    "../src/google/protobuf/stubs/atomicops.h"
    "../src/google/protobuf/stubs/atomicops_internals_x86_msvc.h"
    "../src/google/protobuf/stubs/common.h"
    "../src/google/protobuf/stubs/hash.h"
    "../src/google/protobuf/stubs/map_util.h"
    "../src/google/protobuf/stubs/once.h"
    "../src/google/protobuf/stubs/platform_macros.h"
    "../src/google/protobuf/stubs/stl_util.h"
    "../src/google/protobuf/stubs/stringprintf.h"
    "../src/google/protobuf/stubs/strutil.h"
    "../src/google/protobuf/stubs/substitute.h"
    "../src/google/protobuf/stubs/template_util.h"
    "../src/google/protobuf/stubs/type_traits.h"
    "../src/google/protobuf/text_format.h"
    "../src/google/protobuf/unknown_field_set.h"
    "../src/google/protobuf/wire_format.h"
    "../src/google/protobuf/wire_format_lite.h"
    "../src/google/protobuf/wire_format_lite_inl.h"
    "config.h"
)
source_group("Headers" FILES ${Headers})

set(Sources
    "../src/google/protobuf/compiler/importer.cc"
    "../src/google/protobuf/compiler/parser.cc"
    "../src/google/protobuf/descriptor.cc"
    "../src/google/protobuf/descriptor.pb.cc"
    "../src/google/protobuf/descriptor_database.cc"
    "../src/google/protobuf/dynamic_message.cc"
    "../src/google/protobuf/extension_set.cc"
    "../src/google/protobuf/extension_set_heavy.cc"
    "../src/google/protobuf/generated_message_reflection.cc"
    "../src/google/protobuf/generated_message_util.cc"
    "../src/google/protobuf/io/coded_stream.cc"
    "../src/google/protobuf/io/gzip_stream.cc"
    "../src/google/protobuf/io/printer.cc"
    "../src/google/protobuf/io/strtod.cc"
    "../src/google/protobuf/io/tokenizer.cc"
    "../src/google/protobuf/io/zero_copy_stream.cc"
    "../src/google/protobuf/io/zero_copy_stream_impl.cc"
    "../src/google/protobuf/io/zero_copy_stream_impl_lite.cc"
    "../src/google/protobuf/message.cc"
    "../src/google/protobuf/message_lite.cc"
    "../src/google/protobuf/reflection_ops.cc"
    "../src/google/protobuf/repeated_field.cc"
    "../src/google/protobuf/service.cc"
    "../src/google/protobuf/stubs/atomicops_internals_x86_msvc.cc"
    "../src/google/protobuf/stubs/common.cc"
    "../src/google/protobuf/stubs/once.cc"
    "../src/google/protobuf/stubs/stringprintf.cc"
    "../src/google/protobuf/stubs/structurally_valid.cc"
    "../src/google/protobuf/stubs/strutil.cc"
    "../src/google/protobuf/stubs/substitute.cc"
    "../src/google/protobuf/text_format.cc"
    "../src/google/protobuf/unknown_field_set.cc"
    "../src/google/protobuf/wire_format.cc"
    "../src/google/protobuf/wire_format_lite.cc"
)
source_group("Sources" FILES ${Sources})

set(ALL_FILES
    ${Headers}
    ${Sources}
)

################################################################################
# Target
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
################################################################################
# Output directory
################################################################################
set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${MYROOTDIR}/../../game/bin"
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
    "${MYROOTDIR}/src;"
    "${CMAKE_CURRENT_SOURCE_DIR}/."
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:"
        "_DEBUG"
    ">"
    "$<$<CONFIG:Release>:"
        "NDEBUG"
    ">"
    "_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS;"
    "WIN32;"
    "_WINDOWS;"
    "_USRDLL;"
    "LIBPROTOBUF_EXPORTS"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:
            /Gm;
            /Od;
            /RTC1;
            /ZI
        >
        $<$<CONFIG:Release>:
            /Zi
        >
        /W3;
        /wd4244;
        /wd4267;
        /wd4018;
        /wd4355;
        /wd4800;
        /wd4251;
        /wd4996;
        /wd4146;
        /wd4305;
        ${DEFAULT_CXX_EXCEPTION_HANDLING}
    )
endif()

