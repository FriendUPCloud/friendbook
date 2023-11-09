#!/bin/bash

sudo cp friendos.desktop /usr/share/xsessions/
cp assets/jwmrc ~/.jwmrc
cp friendos.session ~/.xsessionrc
mkdir -p ~/.config
mkdir -p ~/.config/openbox
cp custom.xml ~/.config/openbox/
mkdir -p ~/.config/friendbook
cp assets/postscript.sh ~/.config/friendbook/
