class Server < ApplicationRecord
  has_many :player_counts

  def self.create_server_from_api(server, version)
    addy, port = server[:server_address].split(':')
    create!(address: addy,
            port: port.to_i,
            build: server[:build_number],
            version: version,
            expansions: server[:expansions_mask],

            server_name: server[:server_name],
            server_description: server[:server_description],
            pwc_url: server[:pwc_url],

            module_name: server[:module_name],
            module_description: server[:module_description],
            module_url: server[:module_url],

            players: server[:active_player_count],
            players_max: server[:maximum_player_count],
            online: false,

            elc: server[:elc_enforced],
            gametype: server[:game_type],
            ilr: server[:ilr_enforced],
            level_min: server[:minimum_level],
            level_max: server[:maximum_level],
            localvault: server[:localvault],
            player_pause: server[:player_pause],
            private: server[:private_server],
            pvp: server[:pvp_level],
            single_party: server[:one_party_only])
  end
end
