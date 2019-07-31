#!/bin/bash

tmux start-server
tmux new-session -d -s "ros" 

tmux new-window -t ros:1 roslaunch provider system.launch

sleep 2s

tmux selectw -t ros:0
tmux splitw -h -t 0 -p 50 
tmux splitw -h -t 0 -p 50 
tmux splitw -h -t 0 -p 50 

tmux send-key -t 0 "rostopic echo temperature_provider" C-m 
tmux send-key -t 1 "rostopic echo temperature_consumer" C-m
tmux send-key -t 2 "rostopic echo temperature_publisher" C-m
tmux send-key -t 3 "rostopic echo temperature_subscriber" C-m
tmux attach

