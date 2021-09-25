class CreatePlayerCounts < ActiveRecord::Migration[5.1]
  def change
    create_table :player_counts do |t|
      t.references :server, foreign_key: true
      t.integer :count
    end
  end
end
