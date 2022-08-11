# Copyright 2021 Self Group Ltd. All Rights Reserved.

module SelfMsg

  class Message
    attr_accessor :id, :type, :sender, :recipient, :ciphertext, :message_type, :priority
    attr_reader :timestamp, :offset
  end

end