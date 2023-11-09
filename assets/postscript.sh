#!/bin/bash

log_file="$HOME/friendbook.log"

# Wait till any window is open
while true; do
    if DISPLAY=:0 xdotool search --sync --name "."; then
        echo "A window has been opened." >> "$log_file"
        break
    fi
    sleep 1
done

# Get a list of all open windows
windows=$(DISPLAY=:0 xdotool search --all --name '' 2>/dev/null)

# Check if there are any open windows
if [ -n "$windows" ]; then
    # Activate the first window in the list
    DISPLAY=:0 xdotool windowactivate "$(echo "$windows" | head -n 1)"
else
    echo "No open windows found."  >> "$log_file"
fi

active_window_id=""

while true; do
    # Get the active window ID
    active_window_id=$(DISPLAY=:0 xdotool getactivewindow)
	echo "Active window id now: \"$active_window_id\"..." >> "$log_file"

    # Check if there is an active window
    if [ -n "$active_window_id" ]; then
        # Maximize the active window
        DISPLAY=:0 xdotool windowsize "$active_window_id" 100% 100%
        
		# Set the "CustomAction" property to the active window
		echo "We got window id: \"$active_window_id\" - setting name to \"Screen\"..." >> "$log_file"
		DISPLAY=:0 xdotool set_window --name "Screen" $active_window_id

		# Get the screen resolution
		screen_width=$(xrandr | awk '/\*/ {print $1}' | cut -d 'x' -f1)
		screen_height=$(xrandr | awk '/\*/ {print $1}' | cut -d 'x' -f2)

		echo "Active window dims $screen_width, $screen_height" >> "$log_file"

		# Move and resize the window to cover the entire screen
		DISPLAY=:0 xdotool windowmove $active_window_id 0 0
		DISPLAY=:0 xdotool windowsize $active_window_id $screen_width $screen_height

		# Position in the right layer
		DISPLAY=:0 wmctrl -i -r $active_window_id -b add,below
		DISPLAY=:0 wmctrl -i -a $active_window_id
		DISPLAY=:0 wmctrl -i -r $active_window_id -e 0,0,0,$screen_width,$screen_height
		
		#DISPLAY=:0 openbox --reconfigure
        
        break
    fi

    # Sleep for a short interval (adjust as needed)
    sleep 1
done



echo "Done." >> "$log_file"

