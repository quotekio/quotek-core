from pyquotek.quant import *
from pyquotek.data import *
from pyquotek.ta import *


rlist = [Record(), Record(), Record()]

rlist[0].timestamp = 13393823
rlist[0].value = 3

rlist[1].timestamp = 13393824
rlist[1].value = 4

rlist[2].timestamp = 13393825
rlist[2].value = 6

print "RLIST LEN: {}".format(len(rlist))

recs = Records(rlist)
print recs[0].timestamp
print recs[1].value

print above(recs,.5,.5)

recs0 = Records([0,1,2,1,2])
recs1 = Records([3,4,5,6,7])
recs2 = Records([7,6,5,4,2])

print cross(recs0,recs1)
print cross(recs1,recs2)

print cross_ex(recs1,recs2)

print min(recs2)
print max(recs2)
print average(recs2)

recs3 = Records([4892,4569,4390, 4482,4082,4729, 4889, 4538, 4294, 4567, 4738])
print sma(recs3,5)

recs4 = Records()
for i in range(0,1500):
    recs4.append(i)

print macd(recs4,12,26,9)

