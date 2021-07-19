#include "rice/rice.hpp"
#include <iostream>

using namespace Rice;


void message_init();

extern "C"
void Init_self_msgproto()
{
    std::cout << "INIT!!" << std::endl;
    message_init();
}