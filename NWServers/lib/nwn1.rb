require 'bindata'

module NWServers
  module NWN1
    class RequestDatagram < BinData::Record
      string :datagram
      uint16be :port
    end

    class BNXIResponse < BinData::Record
      endian :little
      string :datagram, read_length: 4 # BNXR
      uint16 :port
      uint8  :unknown
      uint8  :password
      uint8  :min_level
      uint8  :max_level
      uint8  :cur_players
      uint8  :max_players
      uint8  :local_vault
      uint8  :pvp
      uint8  :unknown2
      uint8  :one_party
      uint8  :elc
      uint8  :ilr
      uint8  :unknown3
      uint8  :mod_name_size
      string :mod_name, read_length: :mod_name_size
    end

    class BNESResponse < BinData::Record
      endian :little
      string :datagram, read_length: 5 # BNERU
      uint16 :port
      uint8  :unknown
      uint8  :server_name_length
      string :server_name, read_length: :server_name_length
    end

    class BNDSResponse < BinData::Record
      endian :little
      string :datagram, read_length: 4 # BNDR
      uint16 :port
      uint32 :server_desc_length
      string :server_desc, read_length: :server_desc_length
      uint32 :mod_desc_length
      string :mod_desc, read_length: :mod_desc_length
      uint32 :build_number_len
      string :build_number, read_length: :build_number_len
      uint16 :game_type
    end

    class Datagram
      VALID_DATAGRAMS = %i{bnxi bnds bnes}
      attr_accessor :bnxi, :bnds, :bnes

      def initialize port
        @udp = UDPSocket.new
        @port = port

        @bnxi = RequestDatagram.new
        @bnxi.datagram = 'BNXI'
        @bnxi.port = @port

        @bnes = RequestDatagram.new
        @bnes.datagram = 'BNES'
        @bnes.port = @port

        @bnds = RequestDatagram.new
        @bnds.datagram = 'BNDS'
        @bnds.port = @port
      end

      def send_bnds(server, port)
        @udp.send @bnds.to_binary_s, 0, server, port
        datagram_response = @udp.recvfrom(65536)
        BNDSResponse.read datagram_response[0]
      end

      def send_bnes(server, port)
        @udp.send @bnes.to_binary_s, 0, server, port
        datagram_response = @udp.recvfrom(65536)
        BNESResponse.read datagram_response[0]
      end

      def send_bnxi(server, port)
        @udp.send @bnxi.to_binary_s, 0, server, port
        datagram_response = @udp.recvfrom(65536)
        BNXIResponse.read datagram_response[0]
      end
    end
  end
end
