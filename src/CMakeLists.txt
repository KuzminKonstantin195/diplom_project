add_executable (diplom_project "diplom_project.cpp" 
${MY_INCLUDE_DIR}/Converter.h		"Converter.cpp" 
${MY_INCLUDE_DIR}/SearchServer.h	"SearchServer.cpp"
${MY_INCLUDE_DIR}/InvertedIndex.h	"InvertedIndex.cpp"
)

target_include_directories(diplom_project PUBLIC ${MY_INCLUDE_DIR})

target_link_libraries(diplom_project PRIVATE nlohmann_json::nlohmann_json)
