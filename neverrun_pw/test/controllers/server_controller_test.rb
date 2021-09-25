require 'test_helper'

class ServerControllerTest < ActionDispatch::IntegrationTest
  test "should get index" do
    get servers_url(version: :nwn1)
    assert_response :success
  end

  test "should get show" do
    get server_url(version: :nwn1, id: Server.first.id)
    assert_response :success
  end

  test "should get random" do
    get server_random_url
    assert_response :success
  end
end
