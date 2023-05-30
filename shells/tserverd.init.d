#!/bin/sh
############################################################
# Name       :      /etc/rc.d/init.d/tserverd
# Author     :      Baowei
# Version    :      1.1
# Description:
# Notes      :        
#
############################################################

#
#
# Include the functions declared in the /etc/init.d/functions
# and /etc/sysconfig/rc file
#

. /etc/sysconfig/rc
. $rc_functions

tserver_path=/usr/local/tserver
tserverd="$tserver_path"/tserverd
export LD_LIBRARY_PATH="$tserver_path"/lib:$LD_LIBRARY_PATH

case $1 in
  start)
	boot_mesg "Starting tserverd..."
	startproc tserver tserver $tserverd -d
	;;
  stop)
	boot_mesg "Stopping tserverd..."
	killproc $tserverd
	;;
  reload)
	boot_mesg "Reloading tserverd..."
	reloadproc $tserverd
	;;
  restart)
	$0 stop
	/bin/sleep 1
	$0 start
	;;
  status)
	statusproc $tserverd
	;;
  *)
	echo "Usage: $0 [start|stop|reload|restart|status]"
	exit 1
    ;;
esac

# End of file
