# FriendBook

This repository contains adaptations for offline hardware, making it possible to 
run Friend OS on a laptop or a tablet.


## Friendbridge

Friendbridge is a service designed to manage and report on X11 windows within a 
*nix environment. It operates by first collecting information about existing 
windows in a list. Subsequently, it monitors window events and categorizes them 
based on the associated process IDs. This allows Friendbridge to correlate X11 
windows with the applications responsible for their creation. The gathered data 
is then reported to Friend Core, which, in turn, communicates with the Friend 
Workspace via websockets to provide a seamless user experience in the browser.

### Functionality:

**Window Organization by Process ID:**

Friendbridge organizes windows based on their associated process IDs.
This organization enables the tracking of applications responsible for creating 
X11 windows. This allows the Friend Workspace to show the windows in the 
Workspace dock, and manage the windows via calls to Friend Core.

**Special Friend Workspace Window:**

Friendbridge identifies a special Friend Workspace window acting as a 
"wallpaper" level window. This specific window is placed in the *below layer* of 
the X11 stack.

**Event Listening:**

Friendbridge continuously monitors changes in the X11 window pool.
It employs an event listener to detect window creation, destruction, and other 
relevant events.

**Reporting to Friend Core:**

Detected window events trigger the generation of appropriate report messages.
These report messages are sent to Friend Core, containing details about the 
associated process and window properties.

**Integration with Friend Core:**

Friendbridge's role is to bridge the communication between X11 windows and 
Friend Core. By organizing windows based on process IDs and tracking relevant 
events, Friendbridge ensures accurate reporting to Friend Core, contributing to 
a cohesive and interactive user experience within the Friend Workspace.

