#--------------------------------------------------------------------------
# assignment-4 project
#--------------------------------------------------------------------------

PROJECT(assignment-4)
FILE(GLOB assignment-4_headers code/*.h)
FILE(GLOB assignment-4_sources code/*.cc)

SET(files_assignment-4 ${assignment-4_headers} ${assignment-4_sources})
SOURCE_GROUP("assignment-4" FILES ${files_assignment-4})

ADD_EXECUTABLE(assignment-4 ${files_assignment-4})
TARGET_LINK_LIBRARIES(assignment-4 core render)
ADD_DEPENDENCIES(assignment-4 core render)