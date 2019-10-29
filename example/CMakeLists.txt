project(example LANGUAGES CXX)

add_executable(hello_world hello_world.cpp)
target_link_libraries(hello_world PUBLIC sml)
add_test(example_hello_world hello_world)

add_executable(robot robot.cpp)
target_link_libraries(robot PUBLIC sml)
add_test(example_robot robot)
