require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_message
    d = Base64.decode64('GAAAAAAAEgAkACAAAAAAABwAGAAIAAQAEgAAACAAAAAAAAAAAAAAAAAAAAAAAAAAFAAAABwAAAAkAAAABAAAAHRlc3QHAAAAYWxpY2U6MQAGAAAAdGVzdDoxAAAkAAAAOWEwZjZjNjktMmI2ZS00YjA2LTg3MWUtMGM1YmFhOTJiYzk2AAAAAA')
    m = SelfMsg::Message.new(data: d)
    p m.id
    p m.sender
    p m.recipient
  end

end
