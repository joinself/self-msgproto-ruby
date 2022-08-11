require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_message
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
