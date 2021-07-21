#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/header_generated.h"

using namespace SelfMessaging;

VALUE header_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if (data != Qnil) {
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "header data must be kind of String");
        }

        const uint8_t *data_ptr = (uint8_t *)RSTRING_PTR(data);
        long data_len = RSTRING_LEN(data);

        auto hdr = GetHeader(data_ptr);

        flatbuffers::Verifier verifier(data_ptr, data_len);

        if (!VerifyHeaderBuffer(verifier)) {
            rb_raise(rb_eStandardError, "header buffer is invalid");
        }

        auto id = hdr->id()->c_str();
        auto mtype = hdr->msgtype();

        VALUE idstr = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), idstr);

        VALUE mtypeint = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtypeint);
    }

    return self;
}

void header_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cHeader = rb_define_class_under(cRubySelfMsg, "Header", rb_cObject);

    rb_define_method(cHeader, "initialize", header_initialize, -1);
}

