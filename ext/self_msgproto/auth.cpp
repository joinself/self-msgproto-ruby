#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/auth_generated.h"

using namespace SelfMessaging;

VALUE auth_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if (data != Qnil) {
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "auth data must be kind of String");
        }

        const uint8_t *data_ptr = (uint8_t *)RSTRING_PTR(data);
        long data_len = RSTRING_LEN(data);

        auto auth = GetAuth(data_ptr);

        flatbuffers::Verifier verifier(data_ptr, data_len);

        if (!VerifyAuthBuffer(verifier)) {
            rb_raise(rb_eStandardError, "auth buffer is invalid");
        }

        auto id = auth->id()->c_str();
        auto mtype = auth->msgtype();
        auto token = auth->token()->c_str();
        auto device = auth->device()->c_str();
        auto offset = auth->offset();

        VALUE idstr = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), idstr);

        VALUE mtypeint = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtypeint);

        VALUE offsetint = rb_int2inum(offset);
        rb_ivar_set(self, rb_intern("@offset"), offsetint);

        VALUE tokenstr = rb_str_new(token, std::strlen(token));
        rb_ivar_set(self, rb_intern("@token"), tokenstr);

        VALUE devicestr = rb_str_new(device, std::strlen(device));
        rb_ivar_set(self, rb_intern("@device"), devicestr);
    }

    return self;
}

VALUE auth_to_fb(VALUE self) 
{
    VALUE idv = rb_ivar_get(self, rb_intern("@id"));
    char *idstr = RSTRING_PTR(idv);

    VALUE tokenv = rb_ivar_get(self, rb_intern("@token"));
    char *tokenstr = RSTRING_PTR(tokenv);

    VALUE devicev = rb_ivar_get(self, rb_intern("@device"));
    char *devicestr = RSTRING_PTR(devicev);

    VALUE offsetv = rb_ivar_get(self, rb_intern("@offset"));
    int64_t offset = NUM2INT(offsetv);

    flatbuffers::FlatBufferBuilder builder(1024);

    auto id = builder.CreateString(idstr);
    auto token = builder.CreateString(tokenstr);
    auto device = builder.CreateString(devicestr);

    AuthBuilder auth_builder(builder);

    auth_builder.add_id(id);
    auth_builder.add_msgtype(MsgType_AUTH);
    auth_builder.add_token(token);
    auth_builder.add_device(device);
    auth_builder.add_offset(offset);

    auto auth = auth_builder.Finish();
    builder.Finish(auth);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    VALUE data = rb_str_new((char *)buf, size);

    builder.Release();

    return data;
}

void auth_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cAuth = rb_define_class_under(cRubySelfMsg, "Auth", rb_cObject);

    rb_define_method(cAuth, "initialize", reinterpret_cast< VALUE ( * ) ( ... ) >(auth_initialize), -1);
    rb_define_method(cAuth, "to_fb", reinterpret_cast< VALUE ( * ) ( ... ) >(auth_to_fb), 0);
}

