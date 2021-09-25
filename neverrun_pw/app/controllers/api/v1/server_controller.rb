class Api::V1::ServerController < ApplicationController
  # API

  def all
    render json: Server.all
  end

  def servers_by_version(version)
    render json: Server.where(version: version)
                     .order(players: :desc)
                     .select(:id, :address, :port, :gametype, :players, :players_max, :server_name, :module_name, :online, :version)
  end

  def nwn1
    servers_by_version 1
  end

  def nwn2
    servers_by_version 2
  end

  def nwnee
    servers_by_version 3
  end

  def server_by_address
    render json: Server.where(address: params[:address], port: params[:port_id]).first
  end

  def server_by_id
    render json: Server.find(params[:id])
  end
end
