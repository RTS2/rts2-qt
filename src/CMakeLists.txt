# Tell CMake to create the helloworld executable
add_executable(rts2-qt main.cpp rts2q_device.cpp rts2q_device.h rts2q_cat.cpp rts2q_cat.h rts2q_vizier.cpp rts2q_vizier.h rts2q_fits.cpp rts2q_fits.h)

# Use the Widgets module from Qt 5.
target_link_libraries(rts2-qt Qt5::Widgets)
target_link_libraries(rts2-qt Qt5::Network)
target_link_libraries(rts2-qt nova)
target_compile_features(rts2-qt PRIVATE cxx_range_for)
