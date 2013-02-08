homeautomation
==============

Simple program to poll a TEMPered device, feed a RRD database and drive a Velleman k8055 device all together.

Software prerequisites :
- TEMPered lib ( https://github.com/edorfaus/TEMPered )
- hidapi ( https://github.com/signal11/hidapi )
- libk8055 ( http://sourceforge.net/projects/libk8055/ )
- RRD (could be optional, but I like to keep statistics

Hardware prerequisistes :
- a TEMPered-compatible device
- a Velleman k8055 device

Usage:
- build with cmake, and copy to /usr/local/bin/
- create the database :
  rrdtool create /var/spool/temper/temp.rrd --step 300 DS:temp:GAUGE:600:U:U RRA:AVERAGE:0.5:1:2016
- add a crontab entry like this one :
  * * * * * /usr/local/bin/thermostat



