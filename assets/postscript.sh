#!/bin/bash

log_file="$HOME/friendbook.log"

echo "Starting friendbook session." > "$log_file"


# Get the active window ID
active_window_id=""

# Check if there is an active window
while [ -z "$active_window_id" ] || ! [[ "$active_window_id" =~ ^0x[0-9a-fA-F]+$ ]]; do
    active_window_id=$(xprop -root _NET_ACTIVE_WINDOW | awk '{print $5}')
    if [[ "$active_window_id" =~ ^0x[0-9a-fA-F]+$ ]]; then
        break  # Exit the loop when a valid active window ID is found
    fi
    sleep 1
done

echo "We have an active window." >> "$log_file"

echo "Active window is $active_window_id" >> "$log_file"

# Set the "CustomAction" property to the active window
xdotool set_window --name "CustomAction" $active_window_id

# Get the screen resolution
screen_width=$(xrandr | awk '/\*/ {print $1}' | cut -d 'x' -f1)
screen_height=$(xrandr | awk '/\*/ {print $1}' | cut -d 'x' -f2)

echo "Active window dims $screen_width, $screen_height" >> "$log_file"

# Move and resize the window to cover the entire screen
xdotool windowmove $active_window_id 0 0
xdotool windowsize $active_window_id $screen_width $screen_height

# Position in the right layer
wmctrl -i -r $active_window_id -b add,below
wmctrl -i -a $active_window_id
wmctrl -i -r $active_window_id -e 0,0,0,$screen_width,$screen_height

echo "Done." >> "$log_file"

