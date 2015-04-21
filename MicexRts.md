# Introduction #

I successfully receive and decode FAST-endoded data from MICEX-RTS exchange(http://rts.micex.ru/)

# Details #

Typical InterpretApplication command line is:
```
InterpretApplication -mlisten 0.0.0.0 -multicast address:port -t /path/to/templates.xml
```

They use multiple IP addresses for feeds, but port numbers may be the same,
so if you run multiple InterpretApplication commands for different
IP's but with the same port you'll receive messages from both feeds on
both running programs (when running on Linux)

I am talking about this behaviour:
http://groups.google.com/group/quickfast_users/browse_thread/thread/bc8dee3bbc782155/c2fcb8aed30e6334

Black Jack