QT5_WRAP_UI(RTS2_UIS_H histogramDialog.ui)

# Tell CMake to create the helloworld executable
add_executable(rts2-qt main.cpp config.cpp config.h rts2q_device.cpp rts2q_device.h rts2q_cat.cpp rts2q_cat.h rts2q_vizier.cpp rts2q_vizier.h rts2q_fits.cpp rts2q_fits.h imghdr.h ${RTS2_UIS_H})

# Use the Widgets module from Qt 5.
target_link_libraries(rts2-qt Qt5::Widgets)
target_link_libraries(rts2-qt Qt5::Network)
target_link_libraries(rts2-qt Qt5::Xml)
target_link_libraries(rts2-qt nova)
target_link_libraries(rts2-qt cfitsio)
target_compile_features(rts2-qt PRIVATE cxx_range_for)
