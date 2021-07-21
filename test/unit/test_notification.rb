require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_notification
    d = Base64.decode64('DAAAAAgADAAEAAsACAAAAAgAAAAAAAABJAAAADg0Y2M1M2YwLWQ1NDUtNDk5OC1iOWIxLWRmOTdmZjNkYjRjOQAAAAA')
    h = SelfMsg::Notification.new(data: d)
    p h.id
    p h.type
  end

end


