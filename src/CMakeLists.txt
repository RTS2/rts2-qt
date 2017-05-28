QT5_WRAP_UI(RTS2_UIS_H histogramDialog.ui)

# Tell CMake to create the helloworld executable
add_executable(rts2-qt main.cpp config.cpp config.h rts2q_device.cpp rts2q_device.h rts2q_cat.cpp rts2q_cat.h rts2q_vizier.cpp rts2q_vizier.h rts2q_fits.cpp rts2q_fits.h imghdr.h rts2q_plot.cpp rts2q_plot.h ${RTS2_UIS_H})

# Use the Widgets module from Qt 5.
target_link_libraries(rts2-qt Qt5::Widgets Qt5::Network Qt5::Xml Qt5::PrintSupport)
target_link_libraries(rts2-qt nova)
target_link_libraries(rts2-qt cfitsio)
target_link_libraries(rts2-qt qcustomplot)
target_compile_features(rts2-qt PRIVATE cxx_range_for)
