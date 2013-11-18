mssh
====
This is a fork from original MultiSSH (Bradley Smith's project) 1.2 with Debian Wheezy patches.

MultiSSH is a GTK+ based SSH client designed to connect to multiple servers and issue to same commands to each server, making administering multiple servers less of a chore. There is also functionality to only issue commands to individual servers, and to select a portion of the servers to send commands to.

Extra features
--------------

The main feature of this fork is the ability to specify remote hosts' ports. Example:

```
mssh myserver1:2221 myserver2:64322 myserver3
```

Default port is 22 (SSH)

Building
--------

```
./configure
make
```

Changelog
---------
  - 1.2.1  - Port number can be specified

Credits
-------
  - Bradley Smith  <brad@brad-smith.co.uk> (Founder and Lead Developer)
  - Francisco Licerán <francisco@intelligenia.com> (port parsing patch)
  - Mario J. Barchéin Molina <mario@intelligenia.com> (port parsing patch)
