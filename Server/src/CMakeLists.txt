set(BOOST_INCLUDE_LIBRARIES system log_setup log)

find_package(Boost REQUIRED COMPONENTS ${BOOST_INCLUDE_LIBRARIES})

set(LIBRARIES Boost::system Boost::log_setup Boost::log)

add_executable(${PROJECT_NAME} main.cpp server.cpp socket.cpp chat_member.cpp logger.cpp)

if (Boost_FOUND)
    target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})
else()
    message("Библиотека boost не найдена")
endif ()
add_library(tests chat_member.cpp)

target_include_directories(tests PUBLIC include)