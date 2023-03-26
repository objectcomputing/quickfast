add_library(QuickFAST STATIC)

find_package(Boost REQUIRED)

#Function to Glob all files by extension
function(GET_FILES NAME DIR EXTENSION)
    file(GLOB_RECURSE FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/*${EXTENSION}
            )
    set(${NAME} ${FILES} PARENT_SCOPE)
endfunction()

#CPP Source Files
GET_FILES(Application_CPP Application .cpp)
GET_FILES(Codecs_CPP Codecs .cpp)
GET_FILES(Common_CPP Common .cpp)
GET_FILES(Communication_CPP Communication .cpp)
GET_FILES(Messages_CPP Messages .cpp)

#Header Directory
target_include_directories(QuickFAST PRIVATE .)

target_sources(QuickFAST
        PRIVATE
        ${Application_CPP}
        ${Codecs_CPP}
        ${Common_CPP}
        ${Communication_CPP}
        ${Messages_CPP}
        )

target_compile_definitions(QuickFAST
        PUBLIC
        BOOST_BIND_GLOBAL_PLACEHOLDERS
        ASIOSERVICE_FWD_H)

find_package(Boost COMPONENTS date_time filesystem system thread ...)
find_package(XercesC COMPONENTS sax ...)

target_link_libraries(QuickFAST
        PUBLIC
        ${Boost_LIBRARIES}
        ${XercesC_LIBRARIES}
        )

add_subdirectory(Examples)