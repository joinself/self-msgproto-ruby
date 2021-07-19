#include <iostream>
#include "rice/rice.hpp"
#include "msgproto/message_generated.h"

using namespace Rice;

Object message_initialize(Object self, VALUE *argv)
{
    int argc;
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if(data != Qnil){
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "message data must be kind of String");
        }
    }

    return self;
}

extern "C"
void message_init() {
    Module mod = define_module("SelfMsg");
    Class msg = define_class_under(mod, "Message");


    
    msg.define_method("initialize", &message_initialize);
}

