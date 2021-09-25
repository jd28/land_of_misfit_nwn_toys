require_relative '../lib/nw_servers'
require 'awesome_print'

# Master Server List

# NWN1
ms = NWServers::MasterServerList.new :nwn1

# NWN2:
# ms = NWMasterServerList.new :nwn2

ap ms.all
# ap ms.find_by_name "server name"
# ap ms.find_by_address "server address"
# ap ms.find_by_module "server module"

# NWN1 Datagrams
dg = NWServers::NWN1::Datagram.new 5121
response = dg.send_bnxi '138.201.20.77', 5123
ap response.mod_name

# NWNEE
ap NWServers::NWNEE.all
