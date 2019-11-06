#--------------------------------------------------------------------------
# assignment-5 project
#--------------------------------------------------------------------------

PROJECT(assignment-5)
FILE(GLOB assignment-5_headers code/*.h)
FILE(GLOB assignment-5_sources code/*.cc)
FILE(GLOB resources code/resources/*.*)
FILE(GLOB animations code/animations/*.*)
FILE(GLOB graphics code/graphics/*.*)
FILE(GLOB tinyxml code/tinyxml/*.*)
FILE(GLOB debug code/debug/*.*)

SET(files_assignment-5 
${assignment-5_headers} 
${assignment-5_sources}
${animations}
${graphics}
${tinyxml}
${debug})
SOURCE_GROUP("assignment-5" FILES ${files_assignment-5})

foreach(FILE ${resources})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)

ADD_EXECUTABLE(assignment-5 ${files_assignment-5})
TARGET_LINK_LIBRARIES(assignment-5 core render)
ADD_DEPENDENCIES(assignment-5 core render)
