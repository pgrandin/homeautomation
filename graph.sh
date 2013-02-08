threshold=20
rrdtool graph temp.png                               \
          --end now --start -1d \
          --imgformat PNG                                 \
          --width 500 --height 200                        \
          --color CANVAS#000000                           \
          --color BACK#101010                             \
          --color FONT#C0C0C0                             \
          --color MGRID#80C080                            \
          --color GRID#808020                             \
          --color FRAME#808080                            \
          --color ARROW#FFFFFF                            \
          --color SHADEA#404040                           \
          --color SHADEB#404040                           \
        DEF:temperature=/var/spool/temper/temp.rrd:temp:AVERAGE            \
        CDEF:good=temperature,${threshold},GT,temperature,0,IF \
        CDEF:hot=temperature,${threshold},GT,${threshold},temperature,-,0,IF \
        CDEF:cold=temperature,${threshold},GT,0,temperature,IF \
        AREA:cold#0000FF:"Cold" \
        AREA:good#00FF00:"Good" \
        STACK:hot#AA0000:"Overheat" \
