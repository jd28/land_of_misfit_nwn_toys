require 'test_helper'

class ServerTest < ActiveSupport::TestCase
  test 'Server Has Many Player Counts' do
    server = Server.first
    assert server.player_counts.count > 0
  end
end
