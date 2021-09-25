require 'test_helper'

class Api::V1::ServerControllerTest < ActionDispatch::IntegrationTest
  test "should get all" do
    get api_v1_servers_url
    assert_response :success
  end

  test "should get nwn1" do
    get api_v1_servers_nwn1_url
    assert_response :success
  end

  test "should get nwn2" do
    get api_v1_servers_nwn2_url
    assert_response :success
  end

  test "should get nwnee" do
    get api_v1_servers_nwnee_url
    assert_response :success
  end

  test "should get server_by_id" do
    get api_v1_server_by_id_url(Server.first.id)
    assert_response :success
  end

  test "should get server_by_address" do
    s = Server.first
    get api_v1_server_by_address_url(address: s.address, port_id: s.port)
    assert_response :success
  end
end
