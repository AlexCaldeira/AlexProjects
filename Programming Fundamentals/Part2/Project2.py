'''Alexandre Dinis Pedro Seixas Caldeira numero 85254
Fundamentos da Programacao 2o Projeto
'''

from collections import OrderedDict

'''||Tipos Abstractos de Dados (TAD)||'''


'''Tipo posicao'''

#recebe duas variaveis l para a linha e c para a coluna  
def faz_pos(l,c):
    p = []
    #verifica se elas sao positivas e inteiras 
    if (l >= 0) and (c >= 0) and isinstance(l,int) and isinstance(c, int):
        #mete-as numa lista e cria um elemento do tipo posicao se a condicao anterior de verificar
        p = p + [l] + [c]

    else:
        #apresenta a mensagem de erro a informar o utilizador que os argumentos estao errados
        raise ValueError("faz_pos: argumentos errados")
    return tuple(p)

#recebe um argumento do tipo posicao
def linha_pos(p):
    #retorna o seu primeiro elemento que corresponde a linha
    return p[0]

#recebe um argumento do tipo posicao
def coluna_pos(p):
    #retorna o seu segundo elemento que correponde a coluna
    return p[1]

#recebe um argumento e verifica se ele e do tipo posicao
def e_pos(arg):
    #verifica se o tamanho do argumento e dois e se ambos os seus argumentos sao inteiros e positivos 
    if (len(arg) == 2) and (isinstance(arg[0], int)) and (isinstance(arg[1], int)) and (arg[0] >= 0) and (arg[1] >= 0) :
        #se a condicao se verificar retorna o valor booleano verdadeiro
        return True
    #se a condicao nao se verificar retorna o valor boleano falso
    return False

#recebe dois argumentos do tipo posicao e vai verificar se eles correspondem a mesma posicao
def pos_iguais(p1,p2):
    #verifica se o primeiro e segundo armgumentos de ambos sao iguais
    if (p1[0] == p2[0]) and (p1[1] == p2[1]):
        #se a condicao se verificar retorna o valor booleano verdadeiro
        return True
    #se a condicao nao se verificar retorna o valor boleano falso
    return False


'''Tipo chave'''

#recebe um tuplo de carateres l e uma cadeia de carateres mgc e vai criar uma chave usando a disposicao por linhas
def gera_chave_linhas(l, mgc):
#verifica se a cadeia de carateres e uma string e se todos os carateres sao maisculos e verifica se l e um tuplo de carateres e tem 25 elementos
    if not(isinstance(mgc,str) and mgc.isupper() and isinstance(l,tuple) and len(l)==25):
        #se a condicao nao se verificar retorna ao utilizar um erro dizendo que os argumentos sao errados
        raise ValueError('gera_chave_linhas: argumentos errados')
    #se a condicao se verificar
    else:
        linha = 0
        coluna = 0
        #retira os espacos da cadeia de carateres
        mgc1 = str.replace(mgc,' ','')
        transfm = mgc1
        #inicia um contador
        i=0
        while(i!=25):
            #se a condicao em cima se verificar junta o elemento i do tuplo l na cadeia transfrm
            transfm +=l[i]
            i+=1   
        #passa para uma variavel a cadeia de strings transfm ordenada alfabeticamente    
        na_chave = ''.join(OrderedDict.fromkeys(transfm))
        #reinicia o contador
        i=0
        #verifica se o tamanho da string na_chave e 25
        if(len(na_chave) != 25):
            #se a condicao nao se verificar algum dos argumentos esta errado e retorna a mensagem de erro ao utilizador
            raise ValueError('gera_chave_linhas: argumentos errados')
        #cria uma lista com 5 listas e 5 listas cada uma
        chave = [['' for coluna in range(5)] for linha in range(5)]
        #inica as condicoes para percorrer as posicoes da chave
        while (coluna < 5 ): 
            while(linha < 5 ):         
                #se as condicoes de verificarem na linha e coluna especificada mete o elemento i de na_chave
                chave[coluna][linha]=na_chave[i]
                #incrementa o contador
                i = i + 1
                #incrementa a linha
                linha = linha + 1
            #reinicia o contador das linhas
            linha = 0
            #incrementa o contador das colunas
            coluna = coluna + 1
        #verifica se a chave obtida e do tipo chave
        if e_chave(chave):
            #retorna a chave obtida se tal se verificar 
            return chave
        else:
            #retorna uma mensagem de erro a informar que os argumentos estao errados se tal nao se verificar
            raise ValueError('gera_chave_linhas: argumentos errados')



#recebe como argumentos uma chave c e uma posicao p
def ref_chave(c,p):
    #retorna o elemento da chave c que se encontra na posicao p
    return c[p[0]][p[1]]

#recebe como argumentos uma chave c, uma posicao p e uma letra l
def muda_chave(c,p,l):
    #muda o elemento da chave c que se encontra na posicao p para a letra l
    c[p[0]][p[1]]=l
    #retorna a chave c
    return c

#verifica se o argumentoo e do tipo chave
def e_chave(arg):
    #inicia dois iteradores i e n
    i = 0
    n = 0
    #passa o argumento para o tipo lista e mete-o noutra variavel
    l_list = list(arg)
    
    #verifica se o argumento e do tipo lista
    if not(isinstance(arg, list)):
        return False
    
    #verifica se ele contem 5 elementos
    if not(len(arg) == 5):
        return False
    #inicia as codicoes para percorrer o argumento
    while (n <= 5) :
        while(i < 5):
            #percorrer cada um dos elementos e verifica se eles teem tambem 5 elementos 
            if(len(arg[i]) == 5):
                #incrementa o contador se a condicao se verificar
                i = i + 1

            else :
                return False
        #incrementa o contador dos elementos
        n = n + 1 
        
        #lista que contem o alfabeto inteiro em maisculas 
        letras = ['A','B','C','D','E','F','G','H','I','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z']
        
    #percorrer cada elemento da chave e verifica se ele se encontra na lista letras
    for coluna in range(len(l_list)):
        for linha in range(len(l_list[coluna])):
            if l_list[coluna][linha] in letras:
                #se se encontrar na lista letras remove-o da lista letras
                letras.remove(l_list[coluna][linha])
    #verifica se todos os elementos se encontram na chave, se tal acontecer a lista letras encontra se vazia    
    if(letras != []):
        return False
            
    return True




def string_chave(c): 
    coluna = 0
    linha = 0
    string = ''
    
    #condicoes para percorrer a chave
    while(coluna < 5):
        while(linha < 5):
            #adiciona um espaco a cada elementos da chave 
            string = string + chave[coluna][linha] + ' ' 
            linha = linha + 1   	   
        #se sair do segundo ciclo while estamos no final da linha e queremos fazer passar para a de baixo quando fizermos print
        #entao somamos \n ao ultimo elemento
        string = string +'\n'
        #reinicia o contador das linhas
        linha = 0
        #incrementa o das colunas
        coluna = coluna + 1 

    return string


'''Funcoes'''

def digramas(mens):
    i = 0
    #cria uma veriavel que contem a cadeia de strings mens sem espacos
    digrm = mens.replace(' ', '')
    new_digrm = ''
    
    #condicao para percorrer a cadeia de strings digrm 
    while(i < (len(digrm)-1)):        
        #se existirem dois elementos iguais e seguidos junta o primeiro elemento normalmente e no elemento repetido junta um X em vez do elemento na cadeia nova
        if digrm[i] == digrm[i+1]:
            new_digrm = new_digrm + digrm[i] + 'X'   
        #junta o elemento de digrm normalmente se condicao nao se verificar
        else:
            new_digrm = new_digrm + digrm[i]
        #incrementa o contador que percorre o digrm
        i = i + 1
    
    new_digrm += digrm[i]
    #se o tamanho da cadeia de stings digrm for impar adiciona um X no final dela
    if(len(new_digrm)%2 != 0):
        new_digrm = new_digrm + 'X'         
        
    return new_digrm

#funcao extra para percorrer e retornar a posicao de um elemento na chave
def cod_finder(d,chave):
    linha = 0
    coluna = 0

    while linha < 5:
        while coluna < 5:   

            if d == chave[linha][coluna]:
                return faz_pos(linha,coluna)

            coluna = coluna + 1

        linha = linha + 1           
        coluna = 0

def figura(digrm,chave):
    #guarda as posicoes de cada uma das strings da cadeia de dimensao 2 digrm
    c1 = cod_finder(digrm[0], chave)
    c2 = cod_finder(digrm[1], chave)
    
    #verifica se as suas linhas sao iguais e retorn r se tal acontecer
    if (c1[0]==c2[0]):
        return ('l', c1, c2)
    
    #verifica se as suas colunas sao iguais e retorna c se tal aontecer
    elif (c1[1] == c2[1]):
        return ('c', c1, c2)
    
    #se as colunas e linhas forem ambas diferentes entao a figura do digrama e um rectangulo e a funcao retorna r
    else:
        return ('r', c1, c2)


#recebe dois argumentos do tipo posicao e um inc
def codifica_l(pos1,pos2,inc):
    #passa ambos os argumentos do tipo posicao de tuplo para lista 
    l_pos1 = list(pos1)
    l_pos2 = list(pos2)

    #se o valor de inc for -1 procede a fazer uma desencriptacao
    if (inc == -1):
        if (l_pos1[1] == 0):
            l_pos2[1] = l_pos2[1] - 1
            l_pos1[1] = 4
            #retorna um tuplo que contem as duas posicoes desencriptadas
            return (tuple(l_pos1),tuple(l_pos2))

        if (l_pos2[1] == 0):     
            l_pos2[1] = 4
            l_pos1[1] = l_pos1[1] - 1 
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))   

        if ((l_pos1[1] != 0) and (l_pos2[1] != 0)):
            l_pos1[1] = l_pos1[1] - 1
            l_pos2[1] = l_pos2[1] - 1
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))
        
    #se o valor de inc for 1 procede a fazer uma encriptacao
    elif (inc == 1) :

        if (l_pos1[1] == 4):
            l_pos2[1] = l_pos2[1] + 1
            l_pos1[1] = 0
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))


        if (l_pos2[1] == 4): 
            l_pos1[1] = l_pos1[1] + 1 
            l_pos2[1] = 0
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))            

        if ((l_pos1[1] != 4) and (l_pos2[1] != 4)):
            l_pos1[1] = l_pos1[1] + 1
            l_pos2[1] = l_pos2[1] + 1
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))
  
def codifica_c(pos1,pos2,inc):
    l_pos1 = list(pos1)
    l_pos2 = list(pos2)

    #se o valor de inc for -1 procede a fazer uma desencriptacao 
    if (inc == -1):
        if (l_pos1[0] == 0):
            l_pos2[0] = l_pos2[0] - 1
            l_pos1[0] = 4
            #retorna um tuplo que contem as duas posicoes desencriptadas
            return (tuple(l_pos1),tuple(l_pos2))

        if (l_pos2[0] == 0):     
            l_pos2[0] = 4
            l_pos1[0] = l_pos1[0] - 1
            #retorna um tuplo que contem as duas posicoes desencriptadas
            return (tuple(l_pos1),tuple(l_pos2))    


        if ((l_pos1[0] != 0) and (l_pos2[0] != 0)):
            l_pos1[0] = l_pos1[0] - 1
            l_pos2[0] = l_pos2[0] - 1
            #retorna um tuplo que contem as duas posicoes desencriptadas
            return (tuple(l_pos1),tuple(l_pos2))
  

    #se o valor de inc for 1 procede a fazer uma encriptacao
    elif (inc == 1) :

        if (l_pos1[0] == 4):
            l_pos2[0] = l_pos2[0] + 1
            l_pos1[0] = 0
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))

        if (l_pos2[0] == 4): 
            l_pos1[0] = l_pos1[0] + 1 
            l_pos2[0] = 0
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))      

        if ((l_pos1[0] != 4) and (l_pos2[0] != 4)):
            l_pos1[0] = l_pos1[0] + 1
            l_pos2[0] = l_pos2[0] + 1
            #retorna um tuplo que contem as duas posicoes encriptadas
            return (tuple(l_pos1),tuple(l_pos2))
 
def codifica_r(pos1,pos2):     
    #retorna um tuplo com as colunas das duas posicoes trocadas uma com a outra
    return ((pos1[0],pos2[1]),(pos2[0],pos1[1]))



def codifica_digrama(digrm,chave,inc):
    #transforma o digrama numa figura
    d_figura = figura(digrm, chave)
    
    #--verifica que tipo de figura o digrama vai ser e codifica-o de acordo--#
    if (d_figura[0] == 'l'):
        #como a figura do digrama e uma linha codifica-o utilizando o codificador das linhas
        codifica = codifica_l(d_figura[1], d_figura[2], inc)
        #retorna o digrama que e a soma das strings de cada uma das posicoes
        return str(str(chave[linha_pos(codifica[0])][coluna_pos(codifica[0])]) + (str(chave[linha_pos(codifica[1])][coluna_pos(codifica[1])])))
    
    elif (d_figura[0] == 'c'):
        #como a figura do digrama e uma coluna codifica-o utilizando o codificador das colunas
        codifica = codifica_c(d_figura[1], d_figura[2], inc)
        #retorna o digrama que e a soma das strings de cada uma das posicoes
        return str(str(chave[linha_pos(codifica[0])][coluna_pos(codifica[0])]) + (str(chave[linha_pos(codifica[1])][coluna_pos(codifica[1])])))   
    
    else:
        #como a figura do digrama e um retangulo codifica-o utilizando o codificador dos retangulos
        codifica = codifica_r(d_figura[1], d_figura[2])
        #retorna o digrama que e a soma das strings de cada uma das posicoes
        return str(str(chave[linha_pos(codifica[0])][coluna_pos(codifica[0])]) + (str(chave[linha_pos(codifica[1])][coluna_pos(codifica[1])])))   


def codifica(mens,chave,inc):
    d_digrm = ''
    cod = ''
    i = 0
    
    if (inc == 1) :
        #se quisermos proceder a uma encriptacao primeiro temos de passar a mensagem pelo digramas
        mens = digramas(mens)  
    
    while (i < (len(mens)-1)):
        #cria digramas que sao pares de strings
        d_digrm = mens[i] + mens[i+1]
        #chama a funcao codifica_digrama para os condificar e vai somando as codificacoes todas
        cod = cod + codifica_digrama(d_digrm, chave, inc)
        i = i + 2
    
    return cod

'''------------------------------------GERA_CHAVE_ESPIRAL------------------------------------------'''

def gera_chave_espiral(l,mgc,s,pos):
    s_s = ['r','c']
    pos_s = [(0,0),(0,4),(4,0),(4,4)]
    #verifica se a cadeia de carateres e uma string e se todos os carateres sao maisculos e verifica se l e um tuplo de carateres e tem 25 elementos e 
    #se s e apenas as strings r e c e se pos e um dos 4 cantos da mastriz
    if not(isinstance(mgc,str) and mgc.isupper() and isinstance(l,tuple) and len(l)==25 and (s in s_s) and (pos in pos_s)):
        #retorna mesagem a informar que os argumentos estao errados se a condicao nao se verificar
        raise ValueError('gera_chave_espiral: argumentos errados')
    
    else:
        linha = linha_pos(pos)
        coluna = coluna_pos(pos)      
        mgc1 = str.replace(mgc,' ','')
        transfm = mgc1
        i=0
        while(i!=25):
            transfm +=l[i]
            i+=1   
        na_chave = ''.join(OrderedDict.fromkeys(transfm))
        i=0
        if(len(na_chave) != 25):
            raise ValueError('gera_chave_linhas: argumentos errados')
        chave = [['' for coluna in range(5)] for linha in range(5)]        
        
        if s == 'r':
            if (horizontal_direita(linha,coluna)):
                chave[coluna][linha] = na_chave[i]
                i = i + 1
                coluna = coluna + 1                
                
            elif(vertical_baixo(linha,coluna)):
                chave[coluna][linha] = na_chave[i]
                i = i + 1
                linha = linha + 1                 
            
            elif(horizontal_esquerda(linha,coluna)):
                chave[linha][coluna] = na_chave[i]
                i = i + 1
                coluna = coluna - 1
                
            elif(vertical_cima(linha,coluna)):
                chave[coluna][linha] = na_chave[i]
                i = i + 1
                linha = linha + 1                

                
        
            if e_chave(chave):
                return chave
            else:
                raise ValueError('gera_chave_espiral: argumentos errados')
        
        
        elif s == 'c':''
            
            
        if e_chave(chave):
            return chave
        else:
            raise ValueError('gera_chave_espiral: argumentos errados')
            


def horizontal_direita(linha,coluna):
    if ((coluna == 4) or (chave[coluna][linha]!='')):
        coluna =  coluna - 1
        return False
    else:
        return True
    
    
def vertical_baixo(linha,coluna):
    if ((linha == 4) or (chave[coluna][linha]!='')):
        linha =  linha - 1
        return False
    else:
        return True
    
def horizontal_esquerda(linha,coluna):
    if ((coluna == 0) or (chave[coluna][linha]!='')):
        coluna =  coluna - 1
        return False
    else:
        return True
    
def vertical_cima(linha,coluna):
    if ((linha == 0) or (chave[coluna][linha]!='')):
        return False
    else:
        return True