require "json"
require "net/http"

module NWServers
  class NWNEE
    def self.all
      uri = URI("https://api.nwn.beamdog.net/v1/servers")
      response = Net::HTTP.get(uri)
      JSON.parse(response).map { |node| NWNEE.json_to_server(node) }
    end

    def self.json_to_server(node)
      { active_player_count: node['current_players'],
        build_number: node['build'],
        elc_enforced: node['elc'],
        expansions_mask: 0,
        game_type: node['game_type'],
        ilr_enforced: node['ilr'],
        last_heartbeat: DateTime.strptime(node['last_advertisement'].to_s, '%s'),
        localvault: !node['servervault'],
        maximum_level: node['max_level'],
        maximum_player_count: node['max_players'],
        minimum_level: node['min_level'],
        module_description: node['module_description'],
        module_name: node['module_name'],
        module_url: '',
        one_party_only: node['one_party'],
        online: true,
        pvp_level: node['pvp'],
        pwc_url: '',
        player_pause: node['player_pause'],
        private_server: node['passworded'],
        version: 3,
        server_address: "#{node['host']}:#{node['port']}",
        server_description: '',
        server_name: node['session_name']
      }
    end
  end
end

