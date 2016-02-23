# Handle HDF5
set(HDF5_USE_STATIC_LIBRARIES False CACHE BOOL "HDF5 use static libraries")
find_package(HDF5 COMPONENTS C CXX REQUIRED)
if (NOT ${HDF5_FOUND})
    message(FATAL "Unable to locate HDF5!")
endif()
include_directories(${HDF5_INCLUDE_DIRS})
add_definitions(${HDF5_DEFINITIONS})

# Handle Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(UTILS_LIBRARY_SOURCE_FILES "")
list(APPEND UTILS_LIBRARY_SOURCE_FILES
     BCSimConvenience.hpp
     BCSimConvenience.cpp
     GaussPulse.hpp
     GaussPulse.cpp
     ScanGeometry.hpp
     rotation3d.hpp
     SignalProcessing.hpp
     SignalProcessing.cpp
     HDFConvenience.cpp
     HDFConvenience.hpp
     SimpleHDF.hpp
     cartesianator/Cartesianator.cpp
     cartesianator/Cartesianator.hpp
     )

add_library(LibBCSimUtils STATIC ${UTILS_LIBRARY_SOURCE_FILES})
target_link_libraries(LibBCSimUtils ${HDF5_LIBRARIES})

install(TARGETS LibBCSimUtils DESTINATION lib)
install(FILES HDFConvenience.hpp    DESTINATION include)
install(FILES GaussPulse.hpp        DESTINATION include)
install(FILES BCSimConvenience.hpp  DESTINATION include)
install(FILES SignalProcessing.hpp  DESTINATION include)
install(FILES GaussPulse.hpp        DESTINATION include)