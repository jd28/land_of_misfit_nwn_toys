class CreateServers < ActiveRecord::Migration[5.1]
  def change
    create_table :servers do |t|
      t.string :address, null: false
      t.integer :port, null: false
      t.integer :build, null: false
      t.integer :version, null: false
      t.integer :expansions, null: false

      t.string :server_name, null: false
      t.text :server_description
      t.string :pwc_url

      t.string :module_name, null: false
      t.text :module_description
      t.string :module_url

      t.integer :players, null: false
      t.integer :players_max, null: false

      t.boolean :online, null: false
      t.boolean :elc, null: false
      t.integer :gametype, null: false
      t.boolean :ilr, null: false
      t.integer :level_min, null: false
      t.integer :level_max, null: false
      t.boolean :localvault, null: false
      t.boolean :player_pause, null: false
      t.boolean :private, null: false
      t.integer :pvp, null: false
      t.boolean :single_party, null: false

      t.timestamps
    end
  end
end
