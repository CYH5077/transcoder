set(SOURCES
        DtoResponseJsonType.cpp

        DtoWSFileListRequest.cpp
        DtoWSFileListResponse.cpp

        DtoWSTranscodeProgress.cpp
        DtoWSTranscodeRequest.cpp
        DtoWSTranscodeResponse.cpp

        DtoWSErrorResponse.cpp

)


set(UPDATED_SOURCES "")
foreach(SOURCE IN LISTS SOURCES)
    list(APPEND UPDATED_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
endforeach()


target_sources(${PROJECT_NAME} PRIVATE ${UPDATED_SOURCES})