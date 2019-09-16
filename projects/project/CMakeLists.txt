#--------------------------------------------------------------------------
# project project
#--------------------------------------------------------------------------

PROJECT(project)
FILE(GLOB project_headers code/*.h)
FILE(GLOB project_sources code/*.cc)

SET(files_project ${project_headers} ${project_sources})
SOURCE_GROUP("project" FILES ${files_project})

ADD_EXECUTABLE(project ${files_project})
TARGET_LINK_LIBRARIES(project core render)
ADD_DEPENDENCIES(project core render)