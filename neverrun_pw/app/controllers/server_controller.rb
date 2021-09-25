class ServerController < ApplicationController
  before_action :current_server, only: [:show, :single, :refresh_player_charts]
  before_action :current_version, except: [:all, :single, :random]

  def index
    @servers = Server.where(version: @version).order(players: :desc)
  end

  def random
    @server = Server.order("RANDOM()").first
    @version = @server.version
  end

  def show
  end

  private

  def current_server
    @server = Server.find(params[:id])
  end

  def current_version
    @version = case params[:version].downcase
               when 'nwn1'
                 1
               when 'nwn2'
                 2
               when 'nwnee'
                 3
               end
  end

  def refresh_player_charts
    respond_to do |format|
      format.js
    end
  end
end
