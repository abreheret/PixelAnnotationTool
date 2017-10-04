cmake_minimum_required(VERSION 3.2)
project(PixelAnnotationTool)

set(QT5_DIR "~/Qt/5.9.1/msvc2015_64/lib/cmake" CACHE STRING "Additional search paths for QT5")
set(CMAKE_AUTOMOC ON)
set(PIXEL_ANNOTATION_VERSION "1.0.0")

if(CMAKE_CL_64)
	set(ARCH_TYPE x64)
elseif((CMAKE_GENERATOR MATCHES "ARM") OR ("${arch_hint}" STREQUAL "ARM") OR 	(CMAKE_VS_EFFECTIVE_PLATFORMS MATCHES "ARM|arm"))
	set(ARCH_TYPE ARM)
else()
	set(ARCH_TYPE x86)
endif()


find_path(Qt5Widgets_DIR Qt5WidgetsConfig.cmake PATHS "${QT5_DIR}/Qt5Widgets" )
find_path(Qt5Gui_DIR Qt5GuiConfig.cmake         PATHS "${QT5_DIR}/Qt5Gui"     )
	
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${OpenCV_INCLUDE_DIRS})

qt5_wrap_ui(UI_TEST_HDRS src/main_window.ui )
add_executable(PixelAnnotationTool 
	src/main_window.cpp
	src/main_window.h
	src/about_dialog.cpp
	src/about_dialog.h
	src/labels.cpp 
	src/labels.h
	src/utils.cpp
	src/utils.h
	src/image_mask.cpp
	src/image_mask.h
	src/image_canvas.cpp 
	src/image_canvas.h
	src/label_widget.cpp 
	src/label_widget.h 
	src/main.cpp 
	${UI_TEST_HDRS})
target_link_libraries(PixelAnnotationTool Qt5::Gui Qt5::Widgets ${OpenCV_LIBS})	

if( MSVC )
	set(OpenCV_BIN ${OpenCV_LIB_PATH}/../bin)
	set(NAME_RELEASE PixelAnnotationTool_${ARCH_TYPE}_v${PIXEL_ANNOTATION_VERSION})
	set(DIR_NAME_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/${NAME_RELEASE})
	
	file(COPY 
	${CMAKE_CURRENT_SOURCE_DIR}/images_test/Abbey_Road.jpg 
	${CMAKE_CURRENT_SOURCE_DIR}/images_test/Abbey_Road_mask.png 
	${CMAKE_CURRENT_SOURCE_DIR}/images_test/road1.jpg 
	DESTINATION ${DIR_NAME_RELEASE}/images_test )

	set(DLLs_DEP_RELEASE "${QT5_DIR}/../../bin/Qt5Widgets.dll"
                         "${QT5_DIR}/../../bin/Qt5Gui.dll"
                         "${QT5_DIR}/../../bin/Qt5Core.dll"
                         "${OpenCV_BIN}/opencv_core310.dll"
						 "${OpenCV_BIN}/opencv_imgcodecs310.dll"
						 "${OpenCV_BIN}/opencv_imgproc310.dll")
    set(DLLs_DEP_DEBUG   "${QT5_DIR}/../../bin/Qt5Widgetsd.dll"
                         "${QT5_DIR}/../../bin/Qt5Xmld.dll"
						 "${QT5_DIR}/../../bin/Qt5Guid.dll"
                         "${QT5_DIR}/../../bin/Qt5Cored.dll"
                         "${OpenCV_BIN}/opencv_core310d.dll"
						 "${OpenCV_BIN}/opencv_imgcodecs310d.dll"
						 "${OpenCV_BIN}/opencv_imgproc310d.dll")
    file(COPY ${DLLs_DEP_RELEASE} ${DLLs_DEP_DEBUG} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	
	set( DLLs_DEP_RELEASE_DEPLOYMENT "${CMAKE_CURRENT_BINARY_DIR}/opencv_core310.dll"
							         "${CMAKE_CURRENT_BINARY_DIR}/opencv_imgcodecs310.dll"
							         "${CMAKE_CURRENT_BINARY_DIR}/opencv_imgproc310.dll" )
	
	file(COPY ${DLLs_DEP_RELEASE_DEPLOYMENT} DESTINATION ${DIR_NAME_RELEASE}/bin)
	set_target_properties( PixelAnnotationTool PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR_NAME_RELEASE}/bin )
	add_custom_command(TARGET PixelAnnotationTool POST_BUILD 
	                   COMMAND ${QT5_DIR}/../../bin/windeployqt ${DIR_NAME_RELEASE}/bin
	                   COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${NAME_RELEASE}.zip" --format=zip ${DIR_NAME_RELEASE} )
endif()