'''Alexandre Dinis Pedro Seixas Caldeira numero 85254
Fundamentos da Programacao 1o Projeto
'''

'''A funcao gera_chaves1 recebe como argumento um tuplo com 25 carateres e procede a coloca-los por ordem numa lista 
com 5 linhas e 5 colunas criando uma matriz 5x5, transformando depois essa lista num tuplo que vai ser a chave para desencriptar '''

from math import ceil, sqrt



def gera_chave1(letras):
    linha = 0
    coluna = 0
    n = 0  
    
    chave = [['' for coluna in range(5)] for linha in range(5)]
    
    while (coluna < 5 ):
        
        while(linha < 5 ):
            chave[coluna][linha] = letras[n]
            n = n+1
            linha = linha + 1
         
        chave[coluna]=tuple(chave[coluna])    
        linha = 0
        coluna = coluna + 1
            
    return tuple(chave)    
  
'''A funcao obtem_codigo1 recebe um carater e uma matriz 5x5, a chave, que contem a codificacao do carater, utilizando dois iteradores diferentes, um para
 as colunas e outro para as linhas, procede entao a procurar o carater na matriz e retorna o seu codigo que corresponde o numero da linha juntamente com o da coluna '''
  
def obtem_codigo1(car, chave):
    linha = 0
    coluna = 0
    codigo = ''
    
    while (coluna < 5 ):
                    
        while(linha < 5 ):            
            
            if (chave[coluna][linha] == car):
                codigo = str(coluna) + str(linha)
                            
                return codigo
            
            else:
                linha = linha + 1
                
       
        linha = 0
        coluna = coluna +1
        
'''A funcao codifica1 recebe uma cadeia de carateres e a sua respectiva chave de codificacao, utilizando um iterador que vai sendo incrementado ate ser igual 
ao tamanho da cadeia percorre-a e utilizando a funcao obtem_codigo1 retorna o codigo de cada um deles e soma-os retornando a nova cadeia codificada'''

def codifica1(cad, chave):    
    carater = 0
    codificado = ''
        
    while (carater < len(cad)):
        car = cad[carater]
        codificado = codificado + str(obtem_codigo1(car, chave))
        carater = carater + 1
    
    
    return codificado
        
        
'''A funcao obtem_car1 recebe uma coordenada correspondente a localizacao do carater na matriz chave, o primeiro digito dessa coordenada corresponde a linha e o segundo
a coluna onde esse carater se encontra, utilizando estas coordenadas encontramos o digito na matriz chave e retornamo-lo'''        
def obtem_car1(cod,chave):
    
    linha = int(int(cod)/10)
    coluna = int(int(cod)%10)
    
    car = chave[linha][coluna]
    
    return car

'''A funcao descodifica1 recebe uma cadeia de carateres codificada e a chave para a descodificar, utilizando um interador que percorre a cadeia codificada retiramos as cordenadas 
de cada digito e chamamos a funcao obtem_car1 que retorna o carater correspondente a coordenada e vamos somando as strings de carateres ate termos descodificado a cadeia inteira
e retornamos a soma deles todos como a cadeia descodificada'''

def descodifica1(cad_codificada,chave):
    descodificada = ''
    n = 1      
    cod = ''
    
    while (n < len(cad_codificada)):
                
        cod = str(cad_codificada[n-1]) + str(cad_codificada[n])
        descodificada = descodificada + obtem_car1(cod, chave)
        n = n+2
        
    return descodificada
   
'''A funcao gera_chaves2 recebe como argumento um tuplo de carateres e procede a coloca-los por ordem numa lista em que o numero de tuplos tem de tamanho 
a raiz quadrada do menor quadrado perfeito nao inferior ao numero total de carateres, utilizando dois iteradores para o tuplo letras e outro para o numero de tuplos da chave
vai adicionando na lista chave os membros da lista letras progressivamente, transformando depois essa lista num tuplo que vai ser a chave para desencriptar '''  

def gera_chave2(letras):
    Nelementos = ceil((len(letras))/(ceil(sqrt(len(letras)))))   
    linha = 0
    i = 0
    chave = []
               
    while ( i < Nelementos):
        
        while (linha < ceil(sqrt(len(letras))) ):  
        
            chave = chave + [letras[i:(i + Nelementos )]]
            i = i + Nelementos
            linha = linha + 1                                         
            
    return tuple(chave)   
    
    
'''A funcao obtem_codigo2 recebe como argumento um carater e uma matriz chave que contem os carateres nas sua posicoes de codificacao, utilizando um iterador para 
percorrer as colunas e outro para percorrer as linhas verificamos se o carater se encontra na matriz chave, se tal acontecer retorna a sua codificacao, que consiste
na linha juntamente com a coluna onde se encontra, se nao existir na matriz chave retorna XX'''    

def obtem_codigo2(car, chave):
    
    coluna = 0
    linha = 0      
    codigo = ''
            
    while ( coluna < len(chave)):
            
        while( linha != len(chave[coluna])):
                
            if (chave[coluna][linha] == car):
                codigo = str(coluna) + str(linha)
                    
                return codigo
                
            linha = linha + 1  
        linha=0
        coluna = coluna + 1    
           
    return 'XX'
    
    

'''A funcao codifica2 recebe uma cadeia de carateres a codificar e uma chave que contem a codificacao de carateres, utlizando um iterador para percorrer a cadeia de carateres
obtemos individualmente a codificacao de cada um deles chamando a funcao obtem_codigo2 e retornamos uma cadeia que contem todos os carateres codificados'''     
    
def codifica2(cad, chave):    
    carater = 0
    codificado = ''
        
    while (carater < len(cad)):
        car = cad[carater]
        codificado = codificado + str(obtem_codigo2(car, chave))
        carater = carater + 1
        
    return codificado
    

'''A funcao obtem_car2 recebe uma coordenada e uma matriz chave que contem a codificacao de carateres, procede entao a chamar a funcao obtem_car1 para descobrir 
o carater que se encontra nessa coordenada, se esse carater for o XX retorna entao o carater ? senao retorna o que se encontrar nessa coordenada'''
def obtem_car2 (cod, chave):
    
    if (cod == str('XX')):
                
        return (str('?'))
    
    else:
        return obtem_car1(cod, chave)
    
    
'''A funcao descodifica2 funciona de forma igual a funcao descodifica1 mas em vez de chamar a funcao obtem_car1 para descodificar o carater chama a funcao 
obtem_car2 '''   
def descodifica2(cad_codificada, chave): 
    
    descodificada = ''
    n = 1      
    cod = ''
     
    while (n < len(cad_codificada)):
                 
        cod = str(cad_codificada[n-1]) + str(cad_codificada[n])
         
        descodificada = descodificada + obtem_car2(cod, chave)
         
        n = n+2
         
    return descodificada   