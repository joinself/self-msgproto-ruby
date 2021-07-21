require 'minitest/autorun'
require 'minitest/reporters'
require 'self_msgproto'

reporter_options = { color: true }
Minitest::Reporters.use! [Minitest::Reporters::DefaultReporter.new(reporter_options)]

describe "SelfMsg" do

  describe "Message" do
  
    #let(:msg){ SelfMsg::Message.new(data: "test") }
  
    #describe "#methods" do
  
      #it("returns a Array"){ _(msg.methods).must_be_kind_of Array }
  
    #end
  end
end