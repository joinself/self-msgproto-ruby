require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_header
    d = Base64.decode64('GAAAAAAAEgAkACAAAAAAABwAGAAIAAQAEgAAACAAAAAAAAAAAAAAAAAAAAAAAAAAFAAAABwAAAAkAAAABAAAAHRlc3QHAAAAYWxpY2U6MQAGAAAAdGVzdDoxAAAkAAAAOWEwZjZjNjktMmI2ZS00YjA2LTg3MWUtMGM1YmFhOTJiYzk2AAAAAA')
    h = SelfMsg::Header.new(data: d)
    p h.id
    p h.type
  end

end
