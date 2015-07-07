# This is a script for creating tmux windows and panes for the demo
# Benjamin Toll 06/12/15
# -- Apparently, the -d detach option is necessary for this to work
# -- send-keys simulates writing the keyboard in the given window/pane.
# The command isn't executed unless you type 'C-m'. "\n" doesn't work.
# -- Format panes after all the panes are created.
# -- Layouts are applied to all panes within a given window.
# -- Strange behavior when you try to divide the window
# into more than four panes, with a 'tiled' layout.
# Looks like the command is just ignored.
# -- Adding 'ls -al' after a tmux command doesn't do anything.
# Everything starts up at the same time!

# IMPORTANT NOTES:
# IF YOU CHANGE THE SERVICE OR COMMAND CENTER
# INTERFACES IN ANY WAY, THEN YOU ALSO NEED TO CHANGE THIS FILE.

tmux new-session -s sirius -n jason -d
tmux split-window -t sirius:jason
tmux split-window -t sirius:jason
tmux split-window -t sirius:jason
tmux select-layout main-horizontal
tmux split-window -h -t sirius:jason.0 
tmux send-keys -t sirius:jason.0 \
    "cd command-center; pwd; ./ccserver 5050" C-m
tmux send-keys -t sirius:jason.1 \
    "cd command-center; pwd; node filetransfer_svc.js 5000 5050" C-m
tmux send-keys -t sirius:jason.2 \
    "cd speech-recognition/kaldi/scripts/kaldi-thrift/gen-cpp; pwd; sleep 5; ./server 5100 5050" C-m
tmux send-keys -t sirius:jason.3 \
    "cd image-matching/matching-thrift; pwd; sleep 10; ./run_imserver.sh 5300 5050" C-m
tmux send-keys -t sirius:jason.4 \
    "cd question-answer/openephyra-thrift; pwd; ./start-qa-server-thrift.sh 5200 5050" C-m
tmux attach -t sirius
