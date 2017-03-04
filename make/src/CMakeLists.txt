set(SOURCE_FILES "AETemplate.h" "CloneAtoB.cpp" "CloneAtoB.h" "CloneCircle.cpp" "CloneCircle.h" "CloneGrid.cpp" "CloneGrid.h" "CloneIDs.cpp" "CloneIDs.h" "CloneMesh.cpp" "CloneMesh.h" "ClonerMulti.cpp" "ClonerMulti.h" "ClonerMultiCommand.cpp" "ClonerMultiCommand.h" "ClonerMultiLoc.cpp" "ClonerMultiLoc.h" "CloneSphere.cpp" "CloneSphere.h" "CloneSpline.cpp" "CloneSpline.h" "DagUtils.cpp" "DagUtils.h" "icons.h" "PluginMain.cpp"
)

find_package(Maya REQUIRED)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

MAYA_PLUGIN(${PROJECT_NAME})
