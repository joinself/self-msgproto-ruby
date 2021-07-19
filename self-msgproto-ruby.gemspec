require File.expand_path("../lib/self_msgproto/version", __FILE__)
require 'time'
require 'rake'

Gem::Specification.new do |s|
    s.name    = "self_msgproto"
    s.version = SelfMsg::VERSION
    s.date = Date.today.to_s
    s.summary = "Bindings for Self's Flatbuffer messaging protocol"
    s.authors  = ["Tom Bevan"]
    s.email = "ops@joinself.com"
    s.homepage = "https://github.com/joinself/self-msgproto-ruby"
    s.files = FileList['lib/**/*.rb', 'ext/**/*.{rb,c,h,cpp,hh}', "test/**/*.rb", "Rakefile"]
    s.extensions = ["ext/self_msgproto/extconf.rb"]
    s.license = "MIT"
    s.test_files = Dir.glob("test/**/*.rb")
    s.add_development_dependency 'rice'
    s.add_development_dependency 'rake-compiler'
    s.add_development_dependency 'rake'
    s.add_development_dependency 'minitest'
    s.add_development_dependency 'minitest-reporters'
    s.required_ruby_version = '>= 2.0'
end
