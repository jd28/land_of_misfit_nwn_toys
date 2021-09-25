### nwnx_resman

#### What does it do?

* It replaces the nwserver resource manager.  All Key and Erf files are loaded and handled by the plugin.  There is no longer any limit on the number of a Haks a server can have.
* It changes the `override` to override any resource.  You can use it in the way one traditionally used previous versions of nwnx_resman.
* It allows you to create containers that can load files from anywhere: directories, databases, Redis, etc.
* It can help debug missing resource files.
* It allows you to cache any resource types.  See the `cache` entry in nwnx2.ini.
* It allows you to load Zip files in place of Haks: if you have a hak file named `my_top_hak.hak`, nwnx_resman will look for and prefer `my_top_hak.hak.zip`.  Note this would be the zipped contents of the hak, not the hak itself zipped.  The same is true for key files.

#### Containers

##### Static

Static containers are those whose files do not change.  Examples included are Erf, Key/Bif, and Zip files.  Their precedence is determined by load order.  They are always lower precedence than dynamic containers.

##### Dynamic

These containers are that can load files from any location.  A directory, Redis, a database, etc, etc.  Examples include two different directory containers.  The precedence of these containers is determined solely by the most recent last modification time.

Note it's probably not wise to remove a file once it's been served from a dynamic container.

#### Implementing Your Own Containers

See `containers/Container.h` inherit and implement.  When your plugin is loaded use `SERVICE_RESMAN_ADD_CONTAINER`.  This can also be used in conjunction with the `EVENT_RESMAN_ADD_CONTAINER` which is called whenever nwserver attemps to add a key/erf file or a directory.