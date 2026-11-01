project(uva)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(uva_SRCS uva.cpp main.cpp)
qt4_automoc(${uva_SRCS})
add_executable(uva ${uva_SRCS})
target_link_libraries(uva ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})
install(TARGETS uva RUNTIME DESTINATION bin)
