#!/bin/bash
sudo cp `dirname $0`/libshutdown2.so /usr/lib/dde-dock/plugins/libshutdown2.so
killall dde-dock
