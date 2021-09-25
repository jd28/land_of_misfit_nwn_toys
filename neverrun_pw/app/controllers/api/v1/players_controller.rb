class Api::V1::PlayersController < ApplicationController
  def chart_data
    counts = PlayerCount.where(server_id: params[:id]).order(timestamp: :desc).limit(30).pluck(:count)

    render json: {
        players: counts.reverse,
        popular: get_popular
    }
  end

  private

  def get_popular
    pcounts = PlayerCount.select(:count, :timestamp).where(server_id: params[:id]).all

    data = [
        {count: 0, sum: 0},
        {count: 0, sum: 0},
        {count: 0, sum: 0},
        {count: 0, sum: 0},
        {count: 0, sum: 0},
        {count: 0, sum: 0},
    ]

    pcounts.each do |s|
      case s.timestamp.hour
      when 0..3
        data[0][:count] += 1
        data[0][:sum] += s.count
      when 4..7
        data[1][:count] += 1
        data[1][:sum] += s.count
      when 8..11
        data[2][:count] += 1
        data[2][:sum] += s.count
      when 12..15
        data[3][:count] += 1
        data[3][:sum] += s.count
      when 16..19
        data[4][:count] += 1
        data[4][:sum] += s.count
      when 20..23
        data[5][:count] += 1
        data[5][:sum] += s.count
      end
    end

    { labels: ['12am-4am', '4am-8am', '8am-12pm', '12pm-4pm', '4pm-8pm', '8pm-12am'],
      data: data.map {|t| t[:count] == 0 ? 0 : t[:sum] / t[:count]} }
  end
end
