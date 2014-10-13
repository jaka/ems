#!/bin/bash
SERVER="http://example.com"
CITY="city"

R_TEMP=`./readtemp`
wget --post-data "temperature=${R_TEMP#*T = }" "${SERVER}/${CITY}/put.php" -O /dev/null 2>&1 >/dev/null
