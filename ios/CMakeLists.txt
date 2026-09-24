enable_language(OBJC OBJCXX)

set(THEFT4_BUNDLE_IDENTIFIER "com.lukebrosious.theft4" CACHE STRING "Theft4 app bundle ID")
set(THEFT4_DISPLAY_NAME "Theft4" CACHE STRING "Name shown on the device")

add_library(theft4_core_bridge STATIC
    bridge/theft4_core.cpp
    bridge/theft4_touch_input.cpp
    bridge/theft4_metal_presenter.mm
    bridge/theft4_native_profile_stub.cpp)

target_include_directories(theft4_core_bridge PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/bridge")
target_link_libraries(theft4_core_bridge PRIVATE rex::core)
target_compile_features(theft4_core_bridge PRIVATE cxx_std_23)
set_source_files_properties(bridge/theft4_metal_presenter.mm
    PROPERTIES COMPILE_OPTIONS "-fobjc-arc")

if(THEFT4_LAB_BUILD)
    target_compile_definitions(theft4_core_bridge PRIVATE THEFT4_LAB_BUILD=1)
    if(TARGET rexruntime)
        target_compile_definitions(rexruntime PRIVATE THEFT4_LAB_BUILD=1)
    endif()
endif()

add_executable(Theft4 MACOSX_BUNDLE
    Theft4/main.m
    Theft4/Theft4TouchControls.m
    Theft4/Theft4LauncherView.m
    Theft4/Theft4FrameTimeView.m
    Theft4/Theft4CityView.m
    Theft4/XeniOS-Touch-LICENSE.txt)

set_source_files_properties(Theft4/XeniOS-Touch-LICENSE.txt PROPERTIES
    MACOSX_PACKAGE_LOCATION Resources)

target_link_libraries(Theft4 PRIVATE theft4_core_bridge
    "-framework UIKit"
    "-framework Metal"
    "-framework QuartzCore"
    "-framework SceneKit"
    "-framework CoreGraphics"
    "-framework GameController"
    "-framework CoreHaptics"
    "-framework AVFoundation"
    "-framework AudioToolbox"
    "-framework UniformTypeIdentifiers")

target_link_options(Theft4 PRIVATE "LINKER:-dead_strip")
target_compile_options(Theft4 PRIVATE -fobjc-arc)

set_target_properties(Theft4 PROPERTIES
    LINKER_LANGUAGE CXX
    MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Theft4/Info.plist.in"
    XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${THEFT4_BUNDLE_IDENTIFIER}"
    XCODE_ATTRIBUTE_SKIP_INSTALL NO
    XCODE_ATTRIBUTE_INSTALL_PATH "/Applications"
    XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED NO
    XCODE_ATTRIBUTE_ENABLE_BITCODE NO
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/theft4")

# Keep the bridge symbols available for the app even in a core-only build.
# The native profiler and frame counter are optional runtime features, but the
# shell still calls them during UI setup and FPS display refresh.
