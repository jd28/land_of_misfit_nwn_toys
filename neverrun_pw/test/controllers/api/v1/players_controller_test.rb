require 'test_helper'

class Api::V1::PlayersControllerTest < ActionDispatch::IntegrationTest
  test "should get last_hour" do
    s = Server.first
    get api_v1_players_last_hour_url s.id
    assert_response :success
  end

  test "should get popular" do
    s = Server.first
    get api_v1_players_popular_url s.id
    assert_response :success
  end
end
