// FreeFile.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "global_file.h"
//#include "log.h"
#include "ini.hpp"

boost::shared_ptr<boost::asio::io_service> io_service;
boost::shared_ptr<http::server3::server> http_server;
//BlockManagerPtr block_manager;
//NodeManagerPtr node_manager;
//LogPtr logger;

boost::function0<void> console_ctrl_function;
bool is_running;

BOOL WINAPI console_ctrl_handler(DWORD ctrl_type)
{
  switch (ctrl_type)
  {
  case CTRL_C_EVENT:
  case CTRL_BREAK_EVENT:
  case CTRL_CLOSE_EVENT:
  case CTRL_SHUTDOWN_EVENT:
    console_ctrl_function();
    is_running = false;
    return TRUE;
  default:
    return FALSE;
  }
}

int main(int argc, char* argv[])
{
    unsigned char seed[1024];
    time((std::time_t*)seed);
    *(int*)(seed + sizeof(std::time_t)) = GetTickCount();
    RAND_seed(seed, sizeof(seed));

    Ini ini("config.txt");

    //logger.reset(new Log("log.txt"));
    io_service.reset(new boost::asio::io_service);
    //node_manager.reset(new NodeManager(atoi(ini.get("UDPServer", "Port").c_str())));
    //node_manager->set_options(
    //    atoi(ini.get("NodeManager", "Size").c_str()));
    //node_manager->load();
    //block_manager.reset(new BlockManager);
    //block_manager->set_options(atoi(ini.get("BlockManager", "InMemory").c_str()),
    //    atoi(ini.get("BlockManager", "InDisk").c_str()),
    //    atoi(ini.get("BlockManager", "Downloading").c_str()));
    //block_manager->load();
    http_server.reset(new http::server3::server(*io_service, "0.0.0.0", ini.get("HTTPServer", "Port")));

    //rdr_start();

    // Create a pool of threads to run all of the io_services.
    //std::vector<boost::shared_ptr<boost::thread> > threads;
    //for (std::size_t i = 0; i < 4; ++i)
    //{
    //    boost::shared_ptr<boost::thread> thread(new boost::thread(
    //        boost::bind(&boost::asio::io_service::run, io_service.get())));
    //    threads.push_back(thread);
    //}

   // Set console control handler to allow server to be stopped.
    console_ctrl_function = boost::bind(&boost::asio::io_service::stop, io_service);
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);

	io_service->run();
    //is_running = true;
    //while (is_running) {
    //    Sleep(1000);
    //    node_manager->handle_timer();
    //    block_manager->handle_timer();
    //}

    // Wait for all threads in the pool to exit.
    //for (std::size_t i = 0; i < threads.size(); ++i)
    //    threads[i]->join();

    //rdr_stop();

    //block_manager->save();
    //node_manager->save();

	return 0;
}

