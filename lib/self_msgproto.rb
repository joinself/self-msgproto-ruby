# Copyright 2020 Self Group Ltd. All Rights Reserved.

require 'self_msgproto/version'
require 'self_msgproto/auth'
require 'self_msgproto/header'
require 'self_msgproto/message'
require 'self_msgproto/notification'
require 'self_msgproto/self_msgproto'

module SelfMsg

  MsgTypeMSG = 0
  MsgTypeACK = 1
  MsgTypeERR = 2
  MsgTypeAUTH = 3
  MsgTypeWatch = 5
  MsgTypeStatus = 6
  
end