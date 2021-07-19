require 'minitest/autorun'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_message
    SelfMsg::Message.new(data: "hello")
  end

end
