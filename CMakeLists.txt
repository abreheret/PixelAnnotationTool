cmake_minimum_required(VERSION 3.2)
project(PixelAnnotationTool)



set(QT5_DIR "~/Qt5.9.1/5.9.1/msvc2015_64/lib/cmake" CACHE STRING "Additional search paths for QT5")
set(CMAKE_AUTOMOC ON)

if(WIN32)
	if(CMAKE_CL_64)
		set(ARCH_TYPE x64)
	elseif((CMAKE_GENERATOR MATCHES "ARM") OR ("${arch_hint}" STREQUAL "ARM") OR 	(CMAKE_VS_EFFECTIVE_PLATFORMS MATCHES "ARM|arm"))
		set(ARCH_TYPE ARM)
	else()
		set(ARCH_TYPE x86)
	endif()
elseif(UNIX)
	set(ARCH_TYPE x86_64)
endif()

# ---------------------- MANAGING VERSION NUMBER ----------------------
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/git_version.cmake.in
  ${CMAKE_BINARY_DIR}/git_version.cmake
)
include(${CMAKE_BINARY_DIR}/git_version.cmake)
string(REGEX REPLACE "^v?([0-9]+)\\.([0-9]+)\\.([0-9]+)?(.*)$" "\\1;\\2;\\3;\\4;" REGEX_VERSION ${GIT_TAG} )
LIST(LENGTH REGEX_VERSION n)
if(${n} GREATER 2 )
	list(GET REGEX_VERSION 0 MAJOR_VERSION) 
	list(GET REGEX_VERSION 1 MINOR_VERSION) 
	list(GET REGEX_VERSION 2 PATCH_VERSION) 
	if(${n} GREATER 3 )
		list(GET REGEX_VERSION 3 COMMIT_VERSION) 
		set(PIXEL_ANNOTATION_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}${COMMIT_VERSION}")	
	else()
		set(PIXEL_ANNOTATION_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}")	
	endif()
else() 
	set(MAJOR_VERSION "UnknownMajor") 
	set(MINOR_VERSION "UnknownMinor")  
	set(PATCH_VERSION "UnknownPath") 
	set(PIXEL_ANNOTATION_VERSION ${GIT_TAG})	
	message(WARNING "Version number of pixel annoation tool is warned")
endif()
# ---------------------- MANAGING VERSION NUMBER ----------------------

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
add_custom_command(TARGET PixelAnnotationTool PRE_BUILD COMMAND cmake -P ${CMAKE_BINARY_DIR}/git_version.cmake)

set(OpenCV_BIN ${OpenCV_LIB_PATH}/../bin)
set(NAME_RELEASE PixelAnnotationTool_${ARCH_TYPE}_v${PIXEL_ANNOTATION_VERSION})
set(DIR_NAME_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/${NAME_RELEASE})
set_target_properties( PixelAnnotationTool PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR_NAME_RELEASE}/bin)	
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/images_test/Abbey_Road.jpg 
          ${CMAKE_CURRENT_SOURCE_DIR}/images_test/Abbey_Road_mask.png 
          ${CMAKE_CURRENT_SOURCE_DIR}/images_test/road1.jpg 
          DESTINATION ${DIR_NAME_RELEASE}/images_test )
          
string(REGEX REPLACE "\\." "" CV_VERSION_NUMBER ${OpenCV_VERSION})
if( "${OpenCV_LIBS}" MATCHES "opencv_world" )
  set(OpenCV_MODULE_EXPORT "opencv_world")
else()
  set(OpenCV_MODULE_EXPORT "opencv_core;opencv_imgcodecs;opencv_imgproc")
endif()

if( MSVC )
    set(DLLs_DEP_RELEASE "${QT5_DIR}/../../bin/Qt5Widgets.dll"
                         "${QT5_DIR}/../../bin/Qt5Gui.dll"
                         "${QT5_DIR}/../../bin/Qt5Core.dll")
    set(DLLs_DEP_DEBUG   "${QT5_DIR}/../../bin/Qt5Widgetsd.dll"
                         "${QT5_DIR}/../../bin/Qt5Xmld.dll"
                         "${QT5_DIR}/../../bin/Qt5Guid.dll"
                         "${QT5_DIR}/../../bin/Qt5Cored.dll")
    foreach( _comp ${OpenCV_MODULE_EXPORT})
      set(DLLs_DEP_RELEASE ${DLLs_DEP_RELEASE} "${OpenCV_BIN}/${_comp}${CV_VERSION_NUMBER}.dll")
      set(DLLs_DEP_DEBUG ${DLLs_DEP_DEBUG} "${OpenCV_BIN}/${_comp}${CV_VERSION_NUMBER}d.dll")
      set(DLLs_DEP_RELEASE_DEPLOYMENT ${DLLs_DEP_RELEASE} "${CMAKE_CURRENT_BINARY_DIR}/${_comp}${CV_VERSION_NUMBER}.dll")
    endforeach()
    file(COPY ${DLLs_DEP_RELEASE} ${DLLs_DEP_DEBUG} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${DLLs_DEP_RELEASE_DEPLOYMENT} DESTINATION ${DIR_NAME_RELEASE}/bin)
    add_custom_command(TARGET PixelAnnotationTool POST_BUILD 
                           COMMAND ${QT5_DIR}/../../bin/windeployqt ${DIR_NAME_RELEASE}/bin
                           COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${NAME_RELEASE}.zip" --format=zip ${DIR_NAME_RELEASE} )
elseif(UNIX)
#    IF("${isSystemDir}" STREQUAL "-1")
#    SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
#    ENDIF("${isSystemDir}" STREQUAL "-1")
#    set(opecncv_dep libopencv_imgproc libopencv_imgcodecs libopencv_core)
#    foreach(libcv ${opecncv_dep})
#        file(COPY ${OpenCV_DIR}/../../lib/${libcv}.so.${OpenCV_VERSION} DESTINATION ${DIR_NAME_RELEASE}/bin) 
#	file(RENAME ${DIR_NAME_RELEASE}/bin/${libcv}.so.${OpenCV_VERSION} ${DIR_NAME_RELEASE}/bin/${libcv}.so.${OpenCV_VERSION_MAJOR}.${OpenCV_VERSION_MINOR})
#    endforeach()
    # add_custom_command(TARGET PixelAnnotationTool POST_BUILD 
                          # COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:PixelAnnotationTool> ${DIR_NAME_RELEASE}/bin/PixelAnnotationTool
                          # COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts_to_build/linuxdeployqt ${DIR_NAME_RELEASE}/bin/PixelAnnotationTool -qmake=${QT5_DIR}/../../bin/qmake
                          # COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${NAME_RELEASE}.zip" --format=zip ${DIR_NAME_RELEASE} )
#
endif()

