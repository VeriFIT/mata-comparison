import sys, os
cmd_folder=os.getcwd()
if cmd_folder not in sys.path:
    sys.path.insert(0, cmd_folder)
    
import awalipy