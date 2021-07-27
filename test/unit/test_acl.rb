require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_acl
    d = Base64.decode64('EAAAAAwAFAAMABMACwAEAAwAAAAQAAAAAAAAARQAAAAAAAAEBwAAAHBheWxvYWQAJAAAADk5NDliMWEzLWRhN2QtNGQzYy05OTMxLTlkYzkzMTJhZWI4YQAAAAA')
    h = SelfMsg::Acl.new(data: d)
    p h.id
    p h.type
    p h.command
    p h.payload

    p h.command = SelfMsg::AclCommandPERMIT
    p h.to_fb
  end

end

#EAAAAAwAFAAMABMACwAEAAwAAAAQAAAAAAAAAQwAAAAAAAAEAAAAACQAAAA4M2I1Zjg3Zi0yYTYxLTRiM2YtOWU2Zi0yOTk5OTk3ZDdhNDgAAAAA
