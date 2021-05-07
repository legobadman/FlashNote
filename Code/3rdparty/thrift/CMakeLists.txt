CMAKE_MINIMUM_REQUIRED (VERSION 3.15)

project(thrift)

include_directories(
    ..
    ../boost_1_55_0
)

add_compile_definitions(thrift_EXPORTS)

ADD_LIBRARY(thrift STATIC
    async/TAsyncChannel.cpp
    async/TAsyncChannel.h

    concurrency/BoostMonitor.cpp
    concurrency/BoostMutex.cpp
    concurrency/BoostThreadFactory.cpp
    concurrency/BoostThreadFactory.h
    concurrency/Exception.h
    concurrency/PlatformThreadFactory.h
    concurrency/ThreadManager.cpp
    concurrency/ThreadManager.h
    concurrency/TimerManager.cpp
    concurrency/TimerManager.h
    concurrency/Util.cpp
    concurrency/Util.h

    #cxxfunctional.h

    processor/PeekProcessor.cpp
    processor/PeekProcessor.h
    protocol/TBase64Utils.cpp
    protocol/TBase64Utils.h
    protocol/TBinaryProtocol.h
    protocol/TBinaryProtocol.tcc
    protocol/TDebugProtocol.cpp
    protocol/TDebugProtocol.h
    protocol/TDenseProtocol.cpp
    protocol/TDenseProtocol.h
    protocol/TJSONProtocol.cpp
    protocol/TJSONProtocol.h
    protocol/TProtocol.h
    protocol/TVirtualProtocol.h

    #server/TServer.cpp
    server/TServer.h
    server/TSimpleServer.cpp
    server/TSimpleServer.h
    server/TThreadPoolServer.cpp
    server/TThreadPoolServer.h

    TApplicationException.cpp
    TApplicationException.h
    Thrift.cpp
    Thrift.h
    TProcessor.h

    transport/TBufferTransports.cpp
    transport/TBufferTransports.h
    transport/TFDTransport.cpp
    transport/TFDTransport.h
    transport/TFileTransport.cpp
    transport/TFileTransport.h
    transport/THttpClient.cpp
    transport/THttpClient.h
    transport/THttpServer.cpp
    transport/THttpServer.h
    transport/THttpTransport.cpp
    transport/THttpTransport.h
    transport/TPipe.cpp
    transport/TPipe.h
    transport/TPipeServer.cpp
    transport/TPipeServer.h
    transport/TServerSocket.cpp
    transport/TServerSocket.h
    transport/TServerTransport.h
    transport/TSimpleFileTransport.cpp
    transport/TSimpleFileTransport.h
    transport/TSocket.cpp
    transport/TSocket.h
    #transport/TSSLSocket.cpp
    #transport/TSSLSocket.h
    transport/TTransport.h
    transport/TTransportException.cpp
    transport/TTransportException.h
    transport/TTransportUtils.cpp
    transport/TTransportUtils.h
    transport/TVirtualTransport.h
    windows/config.h
    windows/GetTimeOfDay.cpp
    windows/GetTimeOfDay.h
    windows/Operators.h
    windows/SocketPair.cpp
    windows/SocketPair.h
    windows/TWinsockSingleton.cpp
    windows/TWinsockSingleton.h
    windows/WinFcntl.cpp
    windows/WinFcntl.h
)
