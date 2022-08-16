require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_message
    m = SelfMsg::Message.new
    m.id = "id"
    m.type = MsgTypeMSG
    m.sender = "sender"
    m.recipient = "recipient"
    m.ciphertext = "ciphertext"
    m.message_type = "message_type"
    m.priority = 1

    d = m.to_fb
    p d

    m = SelfMsg::Message.new(data: d)

    p m.id
    p m.type
    p m.offset
    p m.timestamp
    p m.sender
    p m.recipient
    p m.ciphertext
    p m.message_type
    p m.priority
  end

  def test_new_message_with_data
    d = Base64.decode64('HAAAABgALAAoAAAAAAAkACAADAAcAAAACAAEABgAAAAoAAAAAgAAAAAAAAAAAAAAAAAAAAAAAAAUAAAAGAAAACAAAAAoAAAAAAAAAAQAAAB0ZXN0BwAAAGFsaWNlOjEABgAAAHRlc3Q6MQAAJAAAADlhMGY2YzY5LTJiNmUtNGIwNi04NzFlLTBjNWJhYTkyYmM5NgAAAAA')
    m = SelfMsg::Message.new(data: d)

    p m.id
    p m.type
    p m.offset
    p m.timestamp
    p m.sender
    p m.recipient
    p m.ciphertext
    p m.message_type
    p m.priority

    p m.to_fb
  end

end
