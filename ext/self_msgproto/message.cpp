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

        auto msg = GetMessage(data_ptr);

        flatbuffers::Verifier verifier(data_ptr, data_len);

        if (!VerifyMessageBuffer(verifier)) {
            rb_raise(rb_eTypeError, "message buffer is invalid");
        }

        // std::cout << std::strlen(msg->id()->c_str()) << std::endl;

        auto id = msg->id()->c_str();
        auto mtype = msg->msgtype();
        auto stype = msg->subtype();
        auto sender = msg->sender()->c_str();
        auto recipient = msg->recipient()->c_str();
        auto ciphertext = msg->ciphertext();

        VALUE idstr = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), idstr);

        VALUE mtypeint = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtypeint);

        VALUE stypeint = rb_int2inum(stype);
        rb_ivar_set(self, rb_intern("@subtype"), stypeint);

        VALUE senderstr = rb_str_new(sender, std::strlen(sender));
        rb_ivar_set(self, rb_intern("@sender"), senderstr);

        VALUE recipientstr = rb_str_new(recipient, std::strlen(recipient));
        rb_ivar_set(self, rb_intern("@recipient"), recipientstr);

        const u_char *ciphertextdata = ciphertext->data();
        long ciphertextsize = ciphertext->size();

        VALUE ciphertextstr = rb_str_new((const char *)ciphertextdata, ciphertextsize);
        rb_ivar_set(self, rb_intern("@ciphertext"), ciphertextstr);
    }

    return self;
}

static VALUE message_to_fb(VALUE self) 
{
    return self;
}

void message_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cMessage = rb_define_class_under(cRubySelfMsg, "Message", rb_cObject);

    rb_define_method(cMessage, "initialize", message_initialize, -1);
    rb_define_method(cMessage, "to_fb", message_to_fb, 0);
}

