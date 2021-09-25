Rails.application.routes.draw do
  root 'server#index', version: 'nwn1'

  get 'server/random', to: 'server#random'
  get ':version/servers/', to: 'server#index', as: 'servers'
  get ':version/servers/:id', to: 'server#show', as: 'server'
  get ':version/servers/:id/refresh_player_charts', to: 'server#refresh_player_charts'

  get ':version/servers/:server/last_hour', to: 'player_count#last_hour', as: 'player_count_last_hour'
  get ':version/servers/:server/popular', to: 'player_count#popular', as: 'player_count_popular'

  namespace :api do
    namespace :v1 do
      get 'servers', to: 'server#all'
      get 'servers/nwn1', to: 'server#nwn1', version: 'nwn1'
      get 'servers/nwn2', to: 'server#nwn2', version: 'nwn2'
      get 'servers/nwnee', to: 'server#nwnee', version: 'nwnee'

      get 'server/:address/:port_id',
          to: 'server#server_by_address',
          as: 'server_by_address',
          constraints: { address: /\d+\.\d+\.\d+\.\d+/,
                         port_id: /\d+/ }
      get 'server/:id', to: 'server#server_by_id', as: 'server_by_id'

      get 'players/:id/chart_data', to: 'players#chart_data', as: 'players_chart_data'
    end
  end
end
