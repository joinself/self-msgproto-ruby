require 'minitest/autorun'
require 'base64'
require 'self_msgproto'

class TestMessage < Minitest::Test

  include SelfMsg

  def test_new_auth
    d = Base64.decode64('EAAAAAwAFAAQAA8ABAAIAAwAAAAYAAAADAAAAAAAAANsAQAAAQAAADEAAABbAQAAZXlKaGJHY2lPaUpGWkVSVFFTSXNJbXRwWkNJNklpSjkuZXlKamFXUWlPaUk1TjJVM1pXUmhZeTAyWW1WaUxUUXpNalV0WVRRNE9DMWxZakkzWm1NNVkyVTBOV1VpTENKbGVIQWlPakUyTWpZNE9UY3lNRElzSW1saGRDSTZNVFl5TmpnNU56RXpOeXdpYVhOeklqb2lkR1Z6ZENJc0ltcDBhU0k2SWpKak9HRmxZalJtTFRreE9EUXROR0ZtWWkxaFpXWXlMVFZqTW1NM01qZzRabUkxWkNJc0luTjFZaUk2SW5SbGMzUWlMQ0owZVhBaU9pSmhkWFJvTG5SdmEyVnVJbjAuM0FlSXRPRzBPMWd1cTVmbjF6TGxZV3ZHelZyTmNSV2djekRFTnk4a05MLXE3NktPRDF4VVJQV2NYSFd1cDlGY3l5Z3k3dHo5cVQ1TjV4SGl0MnlWQ3cAJAAAADdkZmM0MjIzLWU2NzgtNDI1NC1iZmM5LTlkOWQxOGNlYmZjOAAAAAA')
    h = SelfMsg::Auth.new(data: d)
    p h.id
    p h.type
    p h.device
    p h.token
  end

end
