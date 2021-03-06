project(testsource)

set(testsource_SOURCES
	testsourceinput.cpp
	testsourceplugin.cpp
	testsourcethread.cpp
    testsourcesettings.cpp
    testsourcewebapiadapter.cpp
)

set(testsource_HEADERS
	testsourceinput.h
	testsourceplugin.h
	testsourcethread.h
    testsourcesettings.h
    testsourcewebapiadapter.h
)

include_directories(
	${CMAKE_SOURCE_DIR}/swagger/sdrangel/code/qt5/client
)

if(NOT SERVER_MODE)
    set(testsource_SOURCES
        ${testsource_SOURCES}
        testsourcegui.cpp

        testsourcegui.ui
    )
    set(testsource_HEADERS
        ${testsource_HEADERS}
        testsourcegui.h
    )

    set(TARGET_NAME inputtestsource)
    set(TARGET_LIB "Qt5::Widgets")
    set(TARGET_LIB_GUI "sdrgui")
    set(INSTALL_FOLDER ${INSTALL_PLUGINS_DIR})
else()
    set(TARGET_NAME inputtestsourcesrv)
    set(TARGET_LIB "")
    set(TARGET_LIB_GUI "")
    set(INSTALL_FOLDER ${INSTALL_PLUGINSSRV_DIR})
endif()

add_library(${TARGET_NAME} SHARED
	${testsource_SOURCES}
)

target_link_libraries(${TARGET_NAME}
        Qt5::Core
        ${TARGET_LIB}
	sdrbase
	${TARGET_LIB_GUI}
        swagger
)

install(TARGETS ${TARGET_NAME} DESTINATION ${INSTALL_FOLDER})
