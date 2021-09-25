class SanitizeNames < ActiveRecord::Migration[5.1]
  def change
    Server.all.each do |server|
      lame = /\A[0\s!~]*/
      s = server.server_name.strip.gsub(lame, '')
      m = server.module_name.strip.gsub(lame, '')
      server.server_name = (s.length == 0 || s == "<Invalid Server Name>") ? m : s
      server.module_name = m
      server.save!
    end
  end
end
