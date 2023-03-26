GET_FILES(Examples_CPP Examples .cpp)

#TutorialApplication
add_executable(TutorialApplication)
target_include_directories(TutorialApplication
        PRIVATE
        .
        ${CMAKE_SOURCE_DIR}/quickfast/src)

target_sources(TutorialApplication
        PRIVATE
        ${Examples_CPP}
        TutorialApplication/main.cpp
        TutorialApplication/TutorialApplication.cpp)

target_compile_definitions(TutorialApplication
        PUBLIC
        BOOST_BIND_GLOBAL_PLACEHOLDERS
        ASIOSERVICE_FWD_H)

target_link_libraries(TutorialApplication
        PUBLIC
        QuickFAST)

#PerformanceTest
add_executable(PerformanceTest)
target_include_directories(PerformanceTest
        PRIVATE
        .
        ${CMAKE_SOURCE_DIR}/quickfast/src)

target_sources(PerformanceTest
        PRIVATE
        ${Examples_CPP}
        PerformanceTest/main.cpp
        PerformanceTest/MessageCounter.cpp
        PerformanceTest/NullMessage.cpp
        PerformanceTest/PerformanceTest.cpp)

target_compile_definitions(PerformanceTest
        PUBLIC
        BOOST_BIND_GLOBAL_PLACEHOLDERS
        ASIOSERVICE_FWD_H)

target_link_libraries(PerformanceTest
        PUBLIC
        QuickFAST)