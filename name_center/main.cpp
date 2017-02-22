﻿#include <iostream>
#include <string>
#include "yyrpc.h"

using namespace std;

int main(int argc, char *argv[])
{
  InitRpc("./config.cfg",
    [](bool bSucc){ std::cout << "OnRpcInited:" << bSucc << std::endl; }
  );

  getchar();
  return 0;
}