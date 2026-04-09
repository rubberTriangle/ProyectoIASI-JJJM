cmake_minimum_required(VERSION 3.16)

project(ProyectoIASI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ProyectoIASI main.cpp 
    ficheros/HORAPUNTA1.txt ficheros/HORAPUNTA2.txt ficheros/HORAPUNTA3.txt ficheros/HORAPUNTA4.txt ficheros/HORAPUNTA5.txt
    Tablero.h Tablero.cpp
    vehiculo.h vehiculo.cpp
    jugar.h jugar.cpp
    modifiableHeap.h

)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ficheros DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

include(GNUInstallDirs)
install(TARGETS ProyectoIASI
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
