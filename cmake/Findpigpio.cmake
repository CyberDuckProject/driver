# Find include and library paths
find_path(pigpio_INCLUDE_DIR
    NAMES pigpio.h
)
find_library(pigpio_LIBRARY
    NAMES libpigpio.so
)

# Retrieve version number from header
file(STRINGS
    ${pigpio_INCLUDE_DIR}/pigpio.h pigpio_VERSION_MACRO
    REGEX "#define PIGPIO_VERSION [0-9]+"
)
string(REGEX
    MATCH "[0-9]+"
    pigpio_VERSION ${pigpio_VERSION_MACRO}
)

# Handle find_package settings
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(pigpio
    FOUND_VAR pigpio_FOUND
    REQUIRED_VARS
        pigpio_LIBRARY
        pigpio_INCLUDE_DIR
    VERSION_VAR pigpio_VERSION
)

# Create imported target
if(pigpio_FOUND AND NOT TARGET pigpio::pigpio)
    add_library(pigpio::pigpio UNKNOWN IMPORTED)
    set_target_properties(pigpio::pigpio PROPERTIES
        IMPORTED_LOCATION ${pigpio_LIBRARY}
        INTERFACE_INCLUDE_DIRECTORIES ${pigpio_INCLUDE_DIR}
    )
endif()

# Hide cache variables from ccmake
mark_as_advanced(
    pigpio_INCLUDE_DIR
    pigpio_LIBRARY
)
