# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20180305064544) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"

  create_table "player_counts", force: :cascade do |t|
    t.bigint "server_id"
    t.integer "count"
    t.datetime "timestamp"
    t.index ["server_id"], name: "index_player_counts_on_server_id"
  end

  create_table "servers", force: :cascade do |t|
    t.string "address", null: false
    t.integer "port", null: false
    t.integer "build", null: false
    t.integer "version", null: false
    t.integer "expansions", null: false
    t.string "server_name", null: false
    t.text "server_description"
    t.string "pwc_url"
    t.string "module_name", null: false
    t.text "module_description"
    t.string "module_url"
    t.integer "players", null: false
    t.integer "players_max", null: false
    t.boolean "online", null: false
    t.boolean "elc", null: false
    t.integer "gametype", null: false
    t.boolean "ilr", null: false
    t.integer "level_min", null: false
    t.integer "level_max", null: false
    t.boolean "localvault", null: false
    t.boolean "player_pause", null: false
    t.boolean "private", null: false
    t.integer "pvp", null: false
    t.boolean "single_party", null: false
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_foreign_key "player_counts", "servers"
end
