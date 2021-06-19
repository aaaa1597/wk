cmake_minimum_required(VERSION 3.10.2)
project("andcg3dviewer")

add_library( cg3dcom SHARED	CG3DCom.cpp)
add_library( cg3dviewer SHARED	CG3DViewer.cpp
                                Jni.cpp
                                OpenGL/GLES2.cpp
                                )
add_library( mqo STATIC	format/MatrixVector.cpp
                        format/MQO.cpp
                        )
add_library( fbx STATIC	format/MatrixVector.cpp
                        format/FBX.cpp
                        format/import_fbx.cpp
                        )
find_library( log-lib log )
find_library( EGL_LIBRARY EGL )
find_library( GLES_LIBRARY GLESv2 )
find_library( ANDROID_LIBRARY android )

target_link_libraries( mqo cg3dcom )
target_link_libraries( fbx cg3dcom )
target_link_libraries( fbx z )
target_link_libraries( cg3dviewer cg3dcom )
target_link_libraries( cg3dviewer mqo )
target_link_libraries( cg3dviewer fbx )
target_link_libraries( cg3dviewer ${log-lib} )
target_link_libraries( cg3dviewer ${EGL_LIBRARY} )
target_link_libraries( cg3dviewer ${GLES_LIBRARY} )
target_link_libraries( cg3dviewer ${ANDROID_LIBRARY})
