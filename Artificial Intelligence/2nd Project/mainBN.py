# -*- coding: utf-8 -*-
"""
Created on Mon Oct 15 15:53:25 2018

@author: mlopes
"""

import numpy as np
np.set_printoptions(precision=4, suppress=True)

from BN import *
    
gra = [[],[],[0,1],[2],[2]]
ev = (1,1,1,1,1)
    
p1 = Node( np.array([.001]), gra[0] )                   # burglary
print( "p1 false %.4e p1 true %.4e" % (p1.computeProb(ev)[0] , p1.computeProb(ev)[1])) 

p2 = Node( np.array([.002]), gra[1] )                   # earthquake

p3 = Node( np.array([[.001,.29],[.94,.95]]), gra[2] )   # alarm
print( "p1 = 1, p2 = 1, p3 false %.4e p3 true %.4e" % (p3.computeProb(ev)[0] , p3.computeProb(ev)[1])) 

p4 = Node( np.array([.05,.9]), gra[3] )                 # johncalls

p5 = Node( np.array([.01,.7]), gra[4] )                 # marycalls
prob = [p1,p2,p3,p4,p5]

gra = [[],[],[0,1],[2],[2]]
bn = BN(gra, prob)

jp = []
for e1 in [0,1]:
    for e2 in [0,1]:
        for e3 in [0,1]:
            for e4 in [0,1]:
                for e5 in [0,1]:
                    jp.append(bn.computeJointProb((e1, e2, e3, e4, e5)))

print("sum joint %.3f (1)" % sum(jp))

ev = (-1,[],[],1,1)
print("ev : ")
print(ev)
print( "post : %.4g (0.2842)" % bn.computePostProb(ev)  )

ev = ([],-1,[],1,1)
print("ev : ")
print(ev)
print( "post : %.3f (0.176)" % bn.computePostProb(ev)  )

ev = ([],0,1,-1,[])
print("ev : ")
print(ev)
print( "post : %.3f (0.900)" % bn.computePostProb(ev)  )

