
add_test(set_self_thread_name set_self_thread_name set_self_thread_name)
add_executable(set_self_thread_name main.c ../../src/set_self_thread_name.c)
target_link_libraries(set_self_thread_name pthread)
