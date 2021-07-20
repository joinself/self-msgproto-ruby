# Copyright 2021 Self Group Ltd. All Rights Reserved.

module SelfMsg

  class Message
    attr_accessor :id, :type, :subtype, :sender, :recipient, :ciphertext
    attr_reader :timestamp, :offset
  end

end