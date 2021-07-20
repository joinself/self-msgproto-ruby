require 'mkmf-rice'

$CFLAGS = " -std=c99"

RbConfig::MAKEFILE_CONFIG['CC'] = ENV['CC'] if ENV['CC']

pkg_config('flatbuffers')

abort "Missing flatbuffers" unless have_library("flatbuffers")

create_makefile('self_msgproto/self_msgproto')
