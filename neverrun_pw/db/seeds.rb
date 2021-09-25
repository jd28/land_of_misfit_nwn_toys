# This file should contain all the record creation needed to seed the database with its default values.
# The data can then be loaded with the rails db:seed command (or created alongside the database with db:setup).
#
# Examples:
#
#   movies = Movie.create([{ name: 'Star Wars' }, { name: 'Lord of the Rings' }])
#   Character.create(name: 'Luke', movie: movies.first)

require 'nw_servers'

ms = NWServers::MasterServerList.new :nwn1
ms.all.each { |s| Server.create_server_from_api(s, 1) }

ms = NWServers::MasterServerList.new :nwn2
ms.all.each { |s| Server.create_server_from_api(s, 2) }

NWServers::NWNEE.all.each { |s| Server.create_server_from_api(s, 3) }
