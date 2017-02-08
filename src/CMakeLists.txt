# Tell CMake to create the helloworld executable
add_executable(rts2-qt main.cpp rts2q_device.cpp rts2q_device.h)

# Use the Widgets module from Qt 5.
target_link_libraries(rts2-qt Qt5::Widgets)
target_link_libraries(rts2-qt Qt5::Network)
