Gem::Specification.new do |s|
  s.name        = 'nw_servers'
  s.version     = '0.3'
  s.date        = '2018-02-24'
  s.summary     = 'Network library for communicating with Neverwinter Nights Servers and Master Server List'
  s.description = 'Network library for communicating with Neverwinter Nights Servers and Master Server List'
  s.authors     = ["jmd"]
  s.email       = 'jmd2028@gmail.com'
  s.files       = ["lib/nw_servers.rb", 'lib/nwn1.rb', 'lib/nwnee.rb']
  s.homepage    = 'https://github.com/jd28/NWServers'
  s.license     = 'MIT'

  s.add_dependency 'bindata', '~> 2.4'
  s.add_dependency 'nokogiri', '~> 1.8'
  s.add_dependency 'savon', '~> 2.12'

  s.add_development_dependency 'awesome_print', '~> 1.8'
end
