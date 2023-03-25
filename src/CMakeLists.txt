add_library(QuickFAST SHARED)

find_package(Boost REQUIRED)


#Header Directory
target_include_directories(QuickFAST PRIVATE .)

target_sources(QuickFAST
        PRIVATE
        #Common
        Common/QuickFASTPch.cpp
        Common/Types.cpp
        Common/WorkingBuffer.cpp
        Common/Profiler.cpp
        Common/dllmain.cpp
        Common/Decimal.cpp
        Common/BitMap.cpp

        #Communication
        Communication/AsioService.cpp
        Communication/AsynchFileSender.cpp
        Communication/AsynchSender.cpp
        Communication/FileSender.cpp
        Communication/MulticastReceiverHandle.cpp
        Communication/PCapReader.cpp
        )

target_compile_definitions(QuickFAST
        PUBLIC
        ASIOSERVICE_FWD_H)