#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/acl_generated.h"

using namespace SelfMessaging;

VALUE acl_initialize(int argc, VALUE *argv, VALUE self)
{
    VALUE opts;

    (void)rb_scan_args(argc, argv, "0:", &opts);

    opts = (opts == Qnil) ? rb_hash_new(): opts;

    VALUE data = rb_hash_aref(opts, ID2SYM(rb_intern("data")));

    if (data != Qnil) {
        if(rb_obj_is_kind_of(data, rb_cString) != Qtrue){
            rb_raise(rb_eTypeError, "acl data must be kind of String");
        }

        const uint8_t *data_ptr = (uint8_t *)RSTRING_PTR(data);
        long data_len = RSTRING_LEN(data);

        auto acl = GetACL(data_ptr);

        flatbuffers::Verifier verifier(data_ptr, data_len);

        if (!VerifyACLBuffer(verifier)) {
            rb_raise(rb_eStandardError, "acl buffer is invalid");
        }

        auto id = acl->id()->c_str();
        auto mtype = acl->msgtype();
        auto command = acl->command();
        auto payload = acl->payload();

        VALUE idstr = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), idstr);

        VALUE mtypeint = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtypeint);

        VALUE commandint = rb_int2inum(command);
        rb_ivar_set(self, rb_intern("@command"), commandint);

        if (payload != NULL) {
            const u_char *payloaddata = payload->data();
            long payloadsize = payload->size();

            VALUE payloadstr = rb_str_new((const char *)payloaddata, payloadsize);
            rb_ivar_set(self, rb_intern("@payload"), payloadstr);
        }
    }

    return self;
}

VALUE acl_to_fb(VALUE self)
{
    VALUE idv = rb_ivar_get(self, rb_intern("@id"));
    char *idstr = RSTRING_PTR(idv);

    VALUE commandv = rb_ivar_get(self, rb_intern("@command"));
    uint8_t commandint = NUM2INT(commandv);

    ACLCommand command;

    if (commandint > ACLCommand_MAX) {
        rb_raise(rb_eStandardError, "acl command is invalid");
    }

    VALUE payloadv = rb_ivar_get(self, rb_intern("@payload"));

    u_char *payloadstr;
    long payloadlen;

    if (payloadv != Qnil) {
        payloadstr = (u_char *)RSTRING_PTR(payloadv);
        payloadlen = RSTRING_LEN(payloadv);
    }

    flatbuffers::FlatBufferBuilder builder(1024);

    auto id = builder.CreateString(idstr);
    auto payload = builder.CreateVector(payloadstr, payloadlen);

    ACLBuilder acl_builder(builder);

    acl_builder.add_id(id);
    acl_builder.add_msgtype(MsgType_ACL);
    acl_builder.add_command(command);

    if (payloadv != Qnil) {
        acl_builder.add_payload(payload);
    }

    auto acl = acl_builder.Finish();
    builder.Finish(acl);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    VALUE data = rb_str_new((char *)buf, size);

    builder.Release();

    return data;
}

void acl_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cAcl = rb_define_class_under(cRubySelfMsg, "Acl", rb_cObject);

    rb_define_method(cAcl, "initialize", reinterpret_cast< VALUE ( * ) ( ... ) >(acl_initialize), -1);
    rb_define_method(cAcl, "to_fb", reinterpret_cast< VALUE ( * ) ( ... ) >(acl_to_fb), 0);
}

