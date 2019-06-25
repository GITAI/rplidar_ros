// -*- mode: C++ -*-
/*
 *  Copyright (c) 2019, GITAI Inc.
 *  All rights reserved.
 * get_id.cpp
 * Author: Yuki Furuta <me@furushchev.ru>
 */

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <rplidar.h>


using namespace rp::standalone::rplidar;


int main(int argc, char** argv)
{
  std::string serial_port = "/dev/ttyUSB0";
  int serial_baudrate = 225600;

  switch (argc)
  {
  case 2:
    serial_port = argv[1];
    break;
  case 3:
    serial_port = argv[1];
    serial_baudrate = std::atoi(argv[2]);
    break;
  default:
    std::cerr << "Usage: " << argv[0] << " [port] [baudrate]" << std::endl;
    return 1;
  }

  RPlidarDriver* drv = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
  if (!drv)
  {
    std::cerr << "Failed to create driver instance" << std::endl;
    return 1;
  }

  if (IS_FAIL(drv->connect(serial_port.c_str(), (_u32)serial_baudrate))) {
    std::cerr << "Failed to connect to device" << std::endl;
    RPlidarDriver::DisposeDriver(drv);
    return -1;
  }

  rplidar_response_device_info_t devinfo;
  u_result result = drv->getDeviceInfo(devinfo);
  if (IS_FAIL(result))
  {
    if (result == RESULT_OPERATION_TIMEOUT)
      std::cerr << "Operation timed out" << std::endl;
    else
      std::cerr << "Unexpected error (code: " << std::hex << result << ")" << std::endl;
    return 1;
  }

  // print serial number
  for (size_t i = 0; i < 16; ++i)
    std::cout << std::setw(2) << std::setfill('0') << std::hex << devinfo.serialnum[i];
  std::cout << std::endl;

  return 0;
}
