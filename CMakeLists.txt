# Build 

cmake_minimum_required(VERSION 2.8.11)

project(rts2-qt)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5PrintSupport)
find_package(QCustomPlot)
find_library(NOVA nova)

add_subdirectory(src)
