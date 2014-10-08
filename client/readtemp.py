#!/usr/bin/env python
import serial
import httplib
import urllib
import time

sp = serial.Serial(port='/dev/ttyUSB0', baudrate=2400, timeout=2, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS)

def clean_buffer(sp):
  if sp.inWaiting() > 0: sp.read(sp.inWaiting())

def read_temp(sp):
  # Clean buffer
  clean_buffer(sp)
  sp.write(b'T')
  a = sp.read(7)
  clean_buffer(sp)
  return (round(int(a[5:7]+a[3:5], 16) / 16.0, 1))

while True:
  try:
    temp = read_temp(sp)
    print(temp)
    params = urllib.urlencode({'temperature': temp})
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/plain"}
    conn = httplib.HTTPConnection("example.com")
    conn.request("POST", "/city/put.php", params, headers)
    response = conn.getresponse()
    print response.status, response.reason
    data = response.read()
    conn.close()
  except Exception, e:
    print e.__doc__
    print e.message
  time.sleep(30)
