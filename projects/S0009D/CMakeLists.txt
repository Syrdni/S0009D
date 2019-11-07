#--------------------------------------------------------------------------
# S0009D project
#--------------------------------------------------------------------------

PROJECT(S0009D)
FILE(GLOB S0009D_headers code/*.h)
FILE(GLOB S0009D_sources code/*.cc)
FILE(GLOB resources code/resources/*.*)
FILE(GLOB animations code/animations/*.*)
FILE(GLOB graphics code/graphics/*.*)
FILE(GLOB tinyxml code/tinyxml/*.*)
FILE(GLOB debug code/debug/*.*)

SET(files_S0009D 
${S0009D_headers} 
${S0009D_sources}
${animations}
${graphics}
${tinyxml}
${debug})
SOURCE_GROUP("S0009D" FILES ${files_S0009D})

foreach(FILE ${resources})
    configure_file(${FILE} ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
endforeach(FILE)

ADD_EXECUTABLE(S0009D ${files_S0009D})
TARGET_LINK_LIBRARIES(S0009D core render)
ADD_DEPENDENCIES(S0009D core render)
