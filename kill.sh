sudo kill -9 $( ps -ef | grep 'colnago' | grep -v 'grep' | awk '{print $2}' )
