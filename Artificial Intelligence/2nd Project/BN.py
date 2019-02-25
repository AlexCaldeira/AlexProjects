#Alexandre Dinis Pedro Seixas Caldeira Numero 85254, Pedro Mota Veiga Ribeirinho Teixeira Numero 84756, Grupo 47

# -*- coding: utf-8 -*-
"""
Created on Mon Oct 15 15:51:49 2018

@author: mlopes
"""

class Node():
    def __init__(self, prob, parents = []):
        self.parents = parents;
        self.prob = prob;
    
    def computeProb(self, evid):
        n_parents = len(self.parents);
        parents_ev = []
        cprob = [0,0]
        i = 0
        
        if (n_parents == 0): #tem um pai apenas ou nenhum 
            cprob[1]= self.prob[0]
            cprob[0] = 1 - self.prob[0]
            return cprob
        else:
            if (n_parents==1):
                cprob[1] = self.prob[evid[self.parents[0]]]
                cprob[0] = 1 - cprob[1]  
                return cprob
            
            else:
                cprob[1] = self.prob
                while(i < n_parents):
                    cprob[1] = cprob[1][evid[self.parents[i]]]
                    i = i + 1
                aux = cprob[1]
                cprob[1] = aux
                m = 1 - aux
                cprob[0] = m
                return cprob

    
class BN():
    def __init__(self, gra, prob):
        self.gra = gra
        self.prob = prob

    def computePostProb(self, evid):
        nodes = len(self.gra) # numero de nos existentes na rede        
        node_to_compute = 0
        hidden_variables = []
        others = []
        pt = 0 #com o no a calcular a true
        pf = 0 #com o no a calcular a false
        
        for i in range(nodes):
            if (evid[i] == -1):
                node_to_compute = i #P(i|....)
            elif (evid[i] == []):
                hidden_variables.append(i)
            elif ((evid[i] == 0) or (evid[i] == 1)):
                others.append(i)
                
        combinations = []
        n_hidden = len(hidden_variables)
        
        for i in range(1 << n_hidden):
            s=bin(i)[2:]
            s='0'*(n_hidden-len(s))+s
            combinations.append(list(map(int,list(s))))
      
      
        for n in range(len(combinations)):
            evsize = len(evid)
            evt = ()
            evf = ()
            actual = combinations[n]
            aux = [h for h in range(evsize)]
            
            d = 0
            while(d < n_hidden):
                aux[hidden_variables[d]] = actual[d]
                d = d+1
            
            d = 0
            while(d < len(others)):    
                aux[others[d]] = evid[others[d]]
                d = d + 1 
                
            for i in range(evsize):
                if (i == node_to_compute):
                    evt = evt + (1,)
                    evf = evf + (0,)
                else:
                    evt = evt + (aux[i],)
                    evf = evf + (aux[i],)
                    
            pt = pt + self.computeJointProb(evt)
            pf = pf + self.computeJointProb(evf) 
            
            
            
        soma = pt + pf
        alpha = 1 / soma
                        
        return pt*alpha
        
        
    def computeJointProb(self, evid):
        nodes = len(self.gra) # numero de nodes existentes na rede
        jprob = 1
        
        for i in range(nodes):
            p = self.prob[i].computeProb(evid) #prob[i] corresponde a um node
            
            if(evid[i] == 1):
                jprob = jprob*p[1] #multiplica a probabilidade anterior pela a seguir 
                
            elif(evid[i] == 0):
                jprob = jprob*p[0] #multiplica a probabilidade anterior pela a seguir 
                
        return jprob
    
