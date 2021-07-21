#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/notification_generated.h"

using namespace SelfMessaging;

VALUE notification_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if (data != Qnil) {
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "notification data must be kind of String");
        }

        const uint8_t *data_ptr = (uint8_t *)RSTRING_PTR(data);
        long data_len = RSTRING_LEN(data);

        auto ntf = GetNotification(data_ptr);

        flatbuffers::Verifier verifier(data_ptr, data_len);

        if (!VerifyNotificationBuffer(verifier)) {
            rb_raise(rb_eStandardError, "notification buffer is invalid");
        }

        auto id = ntf->id()->c_str();
        auto mtype = ntf->msgtype();
        auto error = ntf->error()->c_str();

        VALUE idstr = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), idstr);

        VALUE mtypeint = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtypeint);

        VALUE errorstr = rb_str_new(error, std::strlen(error));
        rb_ivar_set(self, rb_intern("@error"), errorstr);
    }

    return self;
}

void notification_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cHeader = rb_define_class_under(cRubySelfMsg, "Notification", rb_cObject);

    rb_define_method(cHeader, "initialize", notification_initialize, -1);
}

