#
# Regular cron jobs for the dockerplugin package
#
0 4	* * *	root	[ -x /usr/bin/dockerplugin_maintenance ] && /usr/bin/dockerplugin_maintenance
