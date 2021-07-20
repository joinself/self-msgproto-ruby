#include <iostream>
#include "rice/rice.hpp"
#include "msgproto/message_generated.h"

using namespace Rice;

VALUE message_initialize(int argc, VALUE *argv, VALUE self)
{
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

void message_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cMessage = rb_define_class_under(cRubySelfMsg, "Message", rb_cObject);

    // rb_define_alloc_func(cMessage, _alloc);

    rb_define_method(cMessage, "initialize", message_initialize, -1);

}

