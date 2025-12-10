## main program

-> main
  -> uncorrect arg: exit(1)
  -> parse arg:
    -> --help: print option of web server: exit(0)
    -> --version: print web server version: exit(0)
    -> --auto-config:
      -> generate config file and read it by: error: exit(1), success -->
    -> `PATH/file.json`:                                                |
      -> parse config file and set up serversInfo object <---------------
         to use it to run web server: error: exit(1), success ->
                                                               |
  <-------------------------------------------------------------
  |
  ->