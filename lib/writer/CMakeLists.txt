add_library(writer INTERFACE writer.hpp )

target_link_libraries(writer INTERFACE field)

target_include_directories(writer INTERFACE ${PROJECT_SOURCE_DIR})
