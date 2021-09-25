class AddTimestampToPlayerCounts < ActiveRecord::Migration[5.1]
  def change
    add_column :player_counts, :timestamp, :datetime
  end
end
