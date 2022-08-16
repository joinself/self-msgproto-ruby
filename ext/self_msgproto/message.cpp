#include <iostream>
#include "rice/rice.hpp"
#include "flatbuffers/flatbuffers.h"
#include "msgproto/message_generated.h"

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
            rb_raise(rb_eStandardError, "message buffer is invalid");
        }

        // std::cout << std::strlen(msg->id()->c_str()) << std::endl;

        auto id = msg->id()->c_str();
        auto mtype = msg->msgtype();
        auto sender = msg->sender()->c_str();
        auto recipient = msg->recipient()->c_str();
        auto ciphertext = msg->ciphertext();
        auto metadata = msg->metadata();
        auto message_type = msg->message_type();
        auto priority = msg->priority();

        auto offset = metadata->offset();
        auto timestamp = metadata->timestamp();

        VALUE id_str = rb_str_new(id, std::strlen(id));
        rb_ivar_set(self, rb_intern("@id"), id_str);

        VALUE mtype_int = rb_int2inum(mtype);
        rb_ivar_set(self, rb_intern("@type"), mtype_int);

        VALUE offset_int = rb_int2inum(offset);
        rb_ivar_set(self, rb_intern("@offset"), offset_int);

        VALUE priority_int = rb_int2inum(priority);
        rb_ivar_set(self, rb_intern("@priority"), priority_int);

        VALUE timestamp_int = rb_int2inum(timestamp);
        rb_ivar_set(self, rb_intern("@timestamp"), timestamp_int);

        VALUE sender_str = rb_str_new(sender, std::strlen(sender));
        rb_ivar_set(self, rb_intern("@sender"), sender_str);

        VALUE recipient_str = rb_str_new(recipient, std::strlen(recipient));
        rb_ivar_set(self, rb_intern("@recipient"), recipient_str);

        if (message_type != NULL) {
            const u_char *message_type_data = message_type->data();
            long message_type_size = message_type->size();

            VALUE message_type_str = rb_str_new((const char *)message_type_data, message_type_size);
            rb_ivar_set(self, rb_intern("@message_type"), message_type_str);
        }

        const u_char *ciphertext_data = ciphertext->data();
        long ciphertext_size = ciphertext->size();

        VALUE ciphertext_str = rb_str_new((const char *)ciphertext_data, ciphertext_size);
        rb_ivar_set(self, rb_intern("@ciphertext"), ciphertext_str);
    }

    return self;
}

VALUE message_to_fb(VALUE self) 
{
    flatbuffers::Offset<flatbuffers::Vector<uint8_t>> message_type;
    u_char *message_type_str;
    long message_type_len;

    VALUE id_v = rb_ivar_get(self, rb_intern("@id"));
    char *id_str = RSTRING_PTR(id_v);

    VALUE sender_v = rb_ivar_get(self, rb_intern("@sender"));
    char *sender_str = RSTRING_PTR(sender_v);

    VALUE recipient_v = rb_ivar_get(self, rb_intern("@recipient"));
    char *recipient_str = RSTRING_PTR(recipient_v);

    VALUE priority_v = rb_ivar_get(self, rb_intern("@priority"));
    uint32_t priority = NUM2ULL(priority_v);

    VALUE message_type_v = rb_ivar_get(self, rb_intern("@message_type"));

    if (message_type_v != Qnil) {
        message_type_str = (u_char *)RSTRING_PTR(message_type_v);
        message_type_len = RSTRING_LEN(message_type_v);
    }

    VALUE ciphertext_v = rb_ivar_get(self, rb_intern("@ciphertext"));
    u_char *ciphertext_str = (u_char *)RSTRING_PTR(ciphertext_v);
    long ciphertext_len = RSTRING_LEN(ciphertext_v);

    flatbuffers::FlatBufferBuilder builder(1024);

    auto id = builder.CreateString(id_str);
    auto sender = builder.CreateString(sender_str);
    auto recipient = builder.CreateString(recipient_str);
    auto ciphertext = builder.CreateVector(ciphertext_str, ciphertext_len);

    if (message_type_v != Qnil) {
        message_type = builder.CreateVector(message_type_str, message_type_len);
    }

    SelfMessaging::Metadata metadata(0, 0);

    MessageBuilder msg_builder(builder);

    msg_builder.add_id(id);
    msg_builder.add_msgtype(MsgType_MSG);
    msg_builder.add_sender(sender);
    msg_builder.add_recipient(recipient);
    msg_builder.add_ciphertext(ciphertext);
    msg_builder.add_metadata(&metadata);
    msg_builder.add_priority(priority);

    if (message_type_v != Qnil) {
        msg_builder.add_message_type(message_type);
    }

    auto msg = msg_builder.Finish();
    builder.Finish(msg);

    uint8_t *buf = builder.GetBufferPointer();
    int size = builder.GetSize();

    VALUE data = rb_str_new((char *)buf, size);

    builder.Release();

    return data;
}

void message_init() {
    VALUE cRubySelfMsg = rb_define_module("SelfMsg");
    VALUE cMessage = rb_define_class_under(cRubySelfMsg, "Message", rb_cObject);

    rb_define_method(cMessage, "initialize", reinterpret_cast< VALUE ( * ) ( ... ) >(message_initialize), -1);
    rb_define_method(cMessage, "to_fb", reinterpret_cast< VALUE ( * ) ( ... ) >(message_to_fb), 0);
}

