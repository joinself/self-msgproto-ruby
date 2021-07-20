#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/message_generated.h"

// using namespace Rice;
using namespace SelfMessaging;

VALUE message_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if (data != Qnil) {
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "message data must be kind of String");
        }

        const uint8_t *data_ptr = (uint8_t *)RSTRING_PTR(data);
        long data_len = RSTRING_LEN(data);

        std::cout << "Im running!" << std::endl;

        auto msg = GetMessage(data_ptr);

        auto verifier = flatbuffers::Verifier(data_ptr, data_len);

        if (!VerifyMessageBuffer(*verifier)) {
            rb_raise(rb_eTypeError, "message buffer is invalid");
        } else {
            std::cout << "buffer is ok!" << std::endl;
        }

        auto id = msg->id();
        auto mtype = msg->msgtype();
        auto stype = msg->subtype();
        auto sender = msg->sender();
        auto recipient = msg->recipient();
        auto ciphertext = msg->ciphertext();

        // std::cout << strlen(id->c_str()) << std::endl;

        //VALUE idstr = rb_str_new(id->c_str(), strlen(id->c_str()));
        //rb_ivar_set(self, rb_intern("id"), idstr);
    }

    return self;
}

void message_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cMessage = rb_define_class_under(cRubySelfMsg, "Message", rb_cObject);

    rb_define_method(cMessage, "initialize", message_initialize, -1);
}

