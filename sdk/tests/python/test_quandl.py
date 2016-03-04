import sys
import os

foo = os.getcwd()
sys.path.append(foo)
from pyquotek.datasources import *

q = Quandl("")
dbs = q.get_databases(0)
#print dbs

for d in dbs:
    print d.name
    print d.description



