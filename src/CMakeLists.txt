QT5_WRAP_UI(RTS2_UIS_H histogramDialog.ui loginDialog.ui)

# Tell CMake to create the helloworld executable
add_executable(rts2-qt main.cpp config.cpp config.h qrdevice.cpp qrdevice.h 
	qrcat.cpp qrcat.h qrvizier.cpp qrvizier.h qrfits.cpp qrfits.h imghdr.h 
	qrplot.cpp qrplot.h qrapp.cpp qrapp.h qrlogin.cpp qrlogin.h
	qrwidget.cpp qrwidget.h
	${RTS2_UIS_H}
)

# Use the Widgets module from Qt 5.
target_link_libraries(rts2-qt Qt5::Widgets Qt5::Network Qt5::Xml Qt5::PrintSupport)
target_link_libraries(rts2-qt nova)
target_link_libraries(rts2-qt cfitsio)
target_link_libraries(rts2-qt qcustomplot)
target_compile_features(rts2-qt PRIVATE cxx_range_for)
