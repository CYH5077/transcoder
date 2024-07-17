set(SOURCES
        HTTPFileHandler.cpp
        HTTPWebSocketHandler.cpp
        HTTPSessionManager.cpp

        ws_recv_handler/WSRequestFilter.cpp
        ws_recv_handler/WSRequestTask.cpp 
        ws_recv_handler/WSRequestManager.cpp
        ws_recv_handler/WSClient.cpp
        ws_recv_handler/WSRequestManager.cpp

        ws_recv_handler/WSFileListTask.cpp
        ws_recv_handler/WSTranscodeTask.cpp
)



set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
    list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()


target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})