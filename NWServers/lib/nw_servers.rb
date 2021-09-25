require 'nokogiri'
require 'savon'

require_relative 'nwn1'
require_relative 'nwnee'

module NWServers
  class MasterServerList
    def initialize(product)
      @product = case product
                 when :nwn1
                   'NWN1'
                 when :nwn2
                   'NWN2'
                 else
                   raise ArgumentError, "Only 'NWN1' or 'NWN2' or valid parameters!", product
                 end

      @client = Savon.client(endpoint: 'http://api.mst.valhallalegends.com/NWNMasterServerAPI/NWNMasterServerAPI.svc/ASMX',
                             namespace: 'http://api.mst.valhallalegends.com/NWNMasterServerAPI',
                             convert_request_keys_to: :camelcase,
                             env_namespace: :soap,
                             namespace_identifier: :nwn)
    end

    def all
      response = call('GetOnlineServerList', 'nwn:Product' => @product)
      xml(response).xpath('//NWGameServer').map { |node| node_to_server(node) }
    end

    def find_by_module(mod)
      response = call('LookupServerByModule',
                      'nwn:Product' => @product,
                      'nwn:Module' => mod)
      node_to_server(xml(response).at('NWGameServer'))
    end

    def find_by_name(mod)
      response = call('LookupServerByName',
                      'nwn:Product' => @product,
                      'nwn:ServerName' => mod)
      node_to_server(xml(response).at('NWGameServer'))
    end

    def find_by_address(mod)
      response = call('LookupServerByAddress',
                      'nwn:Product' => @product,
                      'nwn:ServerAddress' => mod)
      node_to_server(xml(response).at('LookupServerByAddressResult'))
    end

    def find_by_type(mod)
      response = call('LookupServerByGameType',
                      'nwn:Product' => @product,
                      'nwn:GameType' => mod)
      xml(response).xpath('//NWGameServer').map { |node| node_to_server(node) }
    end

    def player_count
      response = call('GetOnlineUserCount',
                      'nwn:Product' => @product)
      xml(response).at('GetOnlineUserCountResult').text.to_i
    end

    private

    def call(req, msg = {})
      sact = "http://api.mst.valhallalegends.com/NWNMasterServerAPI/INWNMasterServerAPI/#{req}"
      @client.call(req, message: msg, soap_action: sact)
    end

    def node_to_server(node)
      { active_player_count: node.at('ActivePlayerCount').text.to_i,
        build_number: node.at('BuildNumber').text.to_i,
        elc_enforced: node.at('ELCEnforced').text == 'true',
        expansions_mask: node.at('ExpansionsMask').text.to_i,
        game_type: node.at('GameType').text.to_i,
        ilr_enforced: node.at('ILREnforced').text == 'true',
        last_heartbeat: DateTime.xmlschema(node.at('LastHeartbeat').text),
        localvault: node.at('LocalVault').text == 'true',
        maximum_level: node.at('MaximumLevel').text.to_i,
        maximum_player_count: node.at('MaximumPlayerCount').text.to_i,
        minimum_level: node.at('MinimumLevel').text.to_i,
        module_description: node.at('ModuleDescription').text,
        module_name: node.at('ModuleName').text,
        module_url: node.at('ModuleUrl').text,
        one_party_only: node.at('OnePartyOnly').text == 'true',
        online: node.at('Online').text == 'true',
        pvp_level: node.at('PVPLevel').text.to_i,
        pwc_url: node.at('PWCUrl').text,
        player_pause: node.at('PlayerPause').text == 'true',
        private_server: node.at('PrivateServer').text == 'true',
        version: case node.at('Product').text
                 when "NWN1"
                   1
                 when "NWN2"
                   2
                 else
                   0
                 end,
        server_address: node.at('ServerAddress').text,
        server_description: node.at('ServerDescription').text,
        server_name: node.at('ServerName').text
      }
    end

    def xml(response)
      nxml = Nokogiri::XML(response.http.body)
      nxml.remove_namespaces!
      nxml
    end
  end
end
