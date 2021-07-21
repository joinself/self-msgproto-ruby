#include "rice/rice.hpp"
#include <iostream>

using namespace Rice;

void acl_init();
void auth_init();
void header_init();
void message_init();
void notification_init();


extern "C"
void Init_self_msgproto()
{
    acl_init();
    auth_init();
    header_init();
    message_init();
    notification_init();
}