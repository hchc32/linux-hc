add_executable(timestamp_unittest Timestamp_unittest.cc)
target_link_libraries(timestamp_unittest muduo_base)
add_test(NAME timestamp_unittest COMMAND timestamp_unittest)

add_executable(CountDownLatch_test CountDownLatch_test.cc)
target_link_libraries(CountDownLatch_test muduo_base)

add_executable(CountDownLatch_test2 CountDownLatch_test2.cc)
target_link_libraries(CountDownLatch_test2 muduo_base)

add_executable(BlockingQueue_bench BlockingQueue_bench.cc)
target_link_libraries(BlockingQueue_bench muduo_base)

add_executable(ThreadLocal_test ThreadLocal_test.cc)
target_link_libraries(ThreadLocal_test muduo_base)

add_executable(Log_test1 Log_test1.cc)
target_link_libraries(Log_test1 muduo_base)

add_executable(Log_test2 Log_test2.cc)
target_link_libraries(Log_test2 muduo_base)

if(BOOSTTEST_LIBRARY)
add_executable(LogStream_test LogStream_test.cc)
target_link_libraries(LogStream_test muduo_base boost_unit_test_framework)
endif()

add_executable(LogStream_bench LogStream_bench.cc)
target_link_libraries(LogStream_bench muduo_base)

# add_executable(test test.cc)
# target_link_libraries(test muduo_base)

add_executable(reactor_test01 reactor_test01.cc)
target_link_libraries(reactor_test01 muduo_base muduo_net)

add_executable(reactor_test02 reactor_test02.cc)
target_link_libraries(reactor_test02 muduo_base muduo_net)

add_executable(reactor_test03 reactor_test03.cc)
target_link_libraries(reactor_test03 muduo_base muduo_net)

add_executable(reactor_test04 reactor_test04.cc)
target_link_libraries(reactor_test04 muduo_base muduo_net)

add_executable(reactor_test05 reactor_test05.cc)
target_link_libraries(reactor_test05 muduo_base muduo_net)

add_executable(reactor_test06 reactor_test06.cc)
target_link_libraries(reactor_test06 muduo_base muduo_net)

add_executable(reactor_test07 reactor_test07.cc)
target_link_libraries(reactor_test07 muduo_base muduo_net)

# add_executable(reactor_test08 reactor_test08.cc)
# target_link_libraries(reactor_test08 muduo_base muduo_net)

# add_executable(reactor_test09 reactor_test09.cc)
# target_link_libraries(reactor_test09 muduo_base muduo_net)

# add_executable(reactor_test10 reactor_test10.cc)
# target_link_libraries(reactor_test10 muduo_base muduo_net)

add_executable(reactor_test11 reactor_test11.cc)
target_link_libraries(reactor_test11 muduo_base muduo_net)

add_executable(reactor_test12 reactor_test12.cc)
target_link_libraries(reactor_test12 muduo_base muduo_net)

add_executable(reactor_test13 reactor_test13.cc)
target_link_libraries(reactor_test13 muduo_base muduo_net)

add_executable(tcpclient_test tcpclient_test.cc)
target_link_libraries(tcpclient_test muduo_base muduo_net)









