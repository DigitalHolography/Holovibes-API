set(CUDA_EXTERNAL_INCLUDES
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
    ${nlohmann_json_INCLUDE_DIRS}
)

set(EXTERNAL_INCLUDES
    ${Boost_INCLUDE_DIRS}
    ${CUDA_EXTERNAL_INCLUDES}
)

set(INTERNAL_INCLUDES
    cuda_includes/
    includes/
    includes/api/
    includes/compute/
    includes/data_struct/
    includes/core/
    includes/enum/
    includes/struct/
    includes/io/
    includes/io/input_file/
    includes/io/output_file/
    includes/thread/
    includes/tools/
    includes/tools/cuda_tools/
    includes/tools/units/
    includes/tools/check/
    includes/tools/exception/
    includes/tools/logger/
    includes/tools/state/
    "${CAMERA_DIR}/include/"
)

# ---------------------------------------------------------------------
# The shared library
# ---------------------------------------------------------------------
file(GLOB_RECURSE SOURCES sources/*.cc)
file(GLOB_RECURSE HEADERS includes/*.hh)
file(GLOB_RECURSE CUDA_SOURCES cuda_sources/*.cu)
file(GLOB_RECURSE CUDA_HEADERS cuda_includes/*.cuh)

add_library(Holovibes SHARED
    ${SOURCES}
    ${HEADERS}
    ${CUDA_SOURCES}
    ${CUDA_HEADERS}
)

target_link_libraries(Holovibes PRIVATE
    spdlog::spdlog_header_only
    Boost::program_options
    opencv_videoio
    cufft
    cublas
    cusolver
    nvml
)

target_include_directories(Holovibes PRIVATE
    ${EXTERNAL_INCLUDES}
    ${INTERNAL_INCLUDES}
    ${CUDA_EXTERNAL_INCLUDES}
)

set_target_properties(Holovibes PROPERTIES
    CUDA_ARCHITECTURES "61;75;86;89"
    CUDA_STANDARD 17
)

target_compile_options(Holovibes PRIVATE
    $<$<COMPILE_LANGUAGE:CUDA>:
    --extended-lambda
    -Xcudafe --diag_suppress=field_without_dll_interface
    -diag-suppress 27
    >
    $<$<COMPILE_LANGUAGE:CXX>:
    /W3
    /wd4005
    /wd4003
    >
)

target_compile_definitions(Holovibes PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:.
    _CRT_SECURE_NO_WARNINGS
    >
)

# ---------------------------------------------------------------------
# Post build: Copy necessary files to output dir
# ---------------------------------------------------------------------
add_custom_command(TARGET Holovibes POST_BUILD
    # Copy json patch
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${HOLO_DIR}/assets/json_patches_holofile/" ${CMAKE_BINARY_DIR}/assets/json_patches_holofile/

    # Copy camera libs
    COMMAND ${CMAKE_COMMAND} -E copy "${LIB_DIR}/ximea/xiapi64.dll" ${CMAKE_BINARY_DIR}/xiapi64.dll
    COMMAND ${CMAKE_COMMAND} -E copy "${LIB_DIR}/ids/uEye_api_64.dll" ${CMAKE_BINARY_DIR}/uEye_api_64.dll
    COMMAND ${CMAKE_COMMAND} -E copy "${LIB_DIR}/hamamatsu/dcamapi.dll" ${CMAKE_BINARY_DIR}/dcamapi.dll
)

# ---------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------
install(TARGETS Holovibes
    RUNTIME DESTINATION .
    COMPONENT application
)

install(DIRECTORY "${HOLO_DIR}/assets/json_patches_holofile/"
    DESTINATION assets/json_patches_holofile
    COMPONENT application
)

install(FILES
    ${CMAKE_BINARY_DIR}/CameraHamamatsu.dll
    ${CMAKE_BINARY_DIR}/CameraIds.dll
    ${CMAKE_BINARY_DIR}/CameraOpenCV.dll
    ${CMAKE_BINARY_DIR}/CameraXib.dll
    ${CMAKE_BINARY_DIR}/CameraXiq.dll
    ${CMAKE_BINARY_DIR}/dcamapi.dll
    ${CMAKE_BINARY_DIR}/uEye_api_64.dll
    ${CMAKE_BINARY_DIR}/xiapi64.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cublas64_12.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cublasLt64_12.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cudart64_12.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cufft64_11.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cusolver64_11.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/cusparse64_12.dll
    ${CUDA_TOOLKIT_ROOT_DIR}/bin/nvJitLink_120_0.dll
    DESTINATION .
    COMPONENT application
)

if(HAVE_BFType AND HAVE_BiDef AND HAVE_BiApi AND HAVE_BFErApi)
    install(FILES ${CMAKE_BINARY_DIR}/BitflowCyton.dll
        DESTINATION .
        COMPONENT application
    )
endif()

if(HAVE_BFType AND HAVE_BiDef AND HAVE_BiApi)
    install(FILES ${CMAKE_BINARY_DIR}/CameraAdimec.dll
        DESTINATION .
        COMPONENT application
    )
endif()

if (HAVE_EGrabber AND HAVE_EGrabbers)
    install(FILES
        ${CMAKE_BINARY_DIR}/AmetekS710EuresysCoaxlinkOcto.dll
        ${CMAKE_BINARY_DIR}/AmetekS711EuresysCoaxlinkQsfp+.dll
        ${CMAKE_BINARY_DIR}/AmetekS991EuresysCoaxlinkQsfp+.dll
        ${CMAKE_BINARY_DIR}/CameraPhantomAutoDetection.dll
        DESTINATION .
        COMPONENT application
    )
endif()

if(HAVE_VIMBAX)
    set(VIMBAX_SDK "C:/Program Files/Allied Vision/Vimba X")
    add_custom_command(TARGET Holovibes POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/VmbCPP.dll" ${CMAKE_BINARY_DIR}/VmbCPP.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/VmbC.dll" ${CMAKE_BINARY_DIR}/VmbC.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/GenApi_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/GenApi_MD_VC141_v3_2_AVT.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/GCBase_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/GCBase_MD_VC141_v3_2_AVT.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/XmlParser_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/XmlParser_MD_VC141_v3_2_AVT.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/Log_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/Log_MD_VC141_v3_2_AVT.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/NodeMapData_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/NodeMapData_MD_VC141_v3_2_AVT.dll
        COMMAND ${CMAKE_COMMAND} -E copy "${VIMBAX_SDK}/bin/MathParser_MD_VC141_v3_2_AVT.dll" ${CMAKE_BINARY_DIR}/MathParser_MD_VC141_v3_2_AVT.dll
        )
    install(FILES
        ${CMAKE_BINARY_DIR}/CameraAlvium.dll
        ${CMAKE_BINARY_DIR}/VmbCPP.dll
        ${CMAKE_BINARY_DIR}/VmbC.dll
        ${CMAKE_BINARY_DIR}/GenApi_MD_VC141_v3_2_AVT.dll
        ${CMAKE_BINARY_DIR}/GCBase_MD_VC141_v3_2_AVT.dll
        ${CMAKE_BINARY_DIR}/XmlParser_MD_VC141_v3_2_AVT.dll
        ${CMAKE_BINARY_DIR}/Log_MD_VC141_v3_2_AVT.dll
        ${CMAKE_BINARY_DIR}/NodeMapData_MD_VC141_v3_2_AVT.dll
        ${CMAKE_BINARY_DIR}/MathParser_MD_VC141_v3_2_AVT.dll
        DESTINATION .
        COMPONENT application
    )
endif()

# ---------------------------------------------------------------------
# Packaging
# ---------------------------------------------------------------------
set(INSTALL_DIRECTORY "Holovibes\\\\Backend\\\\${PROJECT_VERSION}")

set(CPACK_GENERATOR "NSIS")

set(CPACK_COMPONENT_APPLICATION_DISPLAY_NAME "Holovibes")
set(CPACK_COMPONENTS_ALL application)

set(CPACK_PACKAGE_INSTALL_DIRECTORY ${INSTALL_DIRECTORY})

set(CPACK_NSIS_MUI_ICON "${HOLO_DIR}/assets/icons/Holovibes.ico")
set(CPACK_NSIS_MUI_UNIICON "${HOLO_DIR}/assets/icons/Holovibes.ico")
set(CPACK_NSIS_INSTALLED_ICON_NAME "Holovibes.dll")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/../LICENSE")

# Add SetCompress off to NSIS script
set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "
    SetCompress off
")

include(CPack)

# ---------------------------------------------------------------------
# Unit testing
# ---------------------------------------------------------------------
# add_subdirectory(tests)
