%Projeto desenvolvido por
%85254	Alexandre Dinis Pedro Seixas Caldeira
%84772	Tiago Miguel Soares Dias
elemento(Lista, I , E) :- elemento(Lista, I, 1, E ).
elemento([P|_], I, I, P).
elemento([_|R], I, AuxI, E) :-
    NAuxI is AuxI +1,
    elemento(R, I, NAuxI, E).

remove_elemento(Lista,E,Nlista):- remove_elemento(Lista,E,[],Nlista).
remove_elemento([],_,Nlistaaux,Nlistaaux).
remove_elemento([P|R],E,Nlistaaux,Nlista) :-
  	E==P ->  
    remove_elemento(R,E,Nlistaaux,Nlista);
    append(Nlistaaux,P, Nlistaaux2),
    remove_elemento(R,E,Nlistaaux2,Nlista).

inverte(Lista,Nlista):-
    inverte(Lista,[],Nlista).
inverte([],Nlistaaux,Nlistaaux).
inverte([P|R],Nlistaaux,Nlista):-
    inverte(R,[P|Nlistaaux],Nlista).

append(Lista,E,Nlista):-
    inverte(Lista,Nlistaaux),
    inverte([E|Nlistaaux],Nlista).

celula(Lab, NLinha, NColuna, Cel) :-
    elemento(Lab, NLinha, Linha),
    elemento(Linha, NColuna, Cel).

celula_visitada([(_,X,Y)|_],X,Y ).
celula_visitada([_|Resto],X,Y):-
    celula_visitada(Resto,X,Y).

movs_possiveis(Lab, Pos_Atual, Movs, Poss):-
    movs_possiveis(Lab, Pos_Atual, Movs, [d,e,b,c],[],Poss).
movs_possiveis(_,_,_,[],PossAux, PossAux).
movs_possiveis(Lab,(X,Y),Movs,[D|R],PossAux,Poss):-
    celula(Lab,X,Y,Cel),
    member(D,Cel),!,
    movs_possiveis(Lab,(X,Y),Movs,R,PossAux,Poss).
movs_possiveis(Lab,(X,Y),Movs,[c|R],PossAux,Poss):-
    NX is X-1,
    celula_visitada(Movs, NX, Y),!,
    movs_possiveis(Lab,(X,Y),Movs,R,PossAux,Poss).
movs_possiveis(Lab,(X,Y),Movs,[b|R],PossAux,Poss):-
    NX is X+1, 
    celula_visitada(Movs, NX, Y),!,
    movs_possiveis(Lab,(X,Y),Movs,R,PossAux,Poss).
movs_possiveis(Lab,(X,Y),Movs,[d|R],PossAux, Poss):-
    NY is Y+1, 
    celula_visitada(Movs,X,NY),!,
    movs_possiveis(Lab,(X,Y),Movs,R,PossAux,Poss).
movs_possiveis(Lab,(X,Y),Movs,[e|R],PossAux,Poss):-
    NY is Y-1, 
    celula_visitada(Movs,X,NY),!,
    movs_possiveis(Lab,(X,Y),Movs,R,PossAux,Poss).
movs_possiveis(Lab,(X,Y),Movs,[c|R],PossAux,Poss):-
    NX is X-1,
    movs_possiveis(Lab,(NX,Y),Movs,R,[(c,NX,Y)|PossAux],Poss).
movs_possiveis(Lab,(X,Y),Movs,[b|R],PossAux,Poss):-
    NX is X+1,
    movs_possiveis(Lab,(X,Y),Movs,R,[(b,NX,Y)|PossAux],Poss).
movs_possiveis(Lab,(X,Y),Movs,[d|R],PossAux,Poss):-
    NY is Y+1,
    movs_possiveis(Lab,(X,Y),Movs,R,[(d,X,NY)|PossAux],Poss).
movs_possiveis(Lab,(X,Y),Movs,[e|R],PossAux,Poss):-
    NY is Y-1,
    movs_possiveis(Lab,(X,Y),Movs,R,[(e,X,NY)|PossAux],Poss).

abs(P1,P2,Abs):-
    (P1-P2) >= 0,
    Abs is (P1-P2).
abs(P1,P2,Abs):-
    (P1-P2) <0,
    Abs is (P2-P1).

distancia((L1,C1),(L2,C2),Dist):-
    abs(L1,L2,Absl),
    abs(C1,C2,Absc),
	Dist is Absl + Absc.

min([Min],Min,_,_ ):-!.
min([(D1,X1,Y1),(_,X2,Y2)|R],Min,I,F):-
    distancia((X1,Y1),F,Dist1),
    distancia((X2,Y2),F,Dist2),
    Dist1 < Dist2,!, 
    min([(D1,X1,Y1)|R],Min,I,F).
min([(_,X1,Y1),(D2,X2,Y2)|R],Min,I,F):-
    distancia((X1,Y1),F,Dist1),
    distancia((X2,Y2),F,Dist2),
    Dist2 < Dist1,!, 
    min([(D2,X2,Y2)|R],Min,I,F).
min([(D1,X1,Y1),(D2,X2,Y2)|R],Min,I,F ):-
    distancia((X1,Y1),F,Dist1),
    distancia((X2,Y2),F,Dist2),
    Dist1 is Dist2,!,
    distancia((X1,Y1),I, Dist3),
    distancia((X2,Y2),I, Dist4),
    Dist3 >= Dist4 ->  
    min([(D1,X1,Y1)|R],Min,I,F);
    min([(D2,X2,Y2)|R],Min,I,F).

ordena_poss(Poss, Poss_ord, Pos_inicial, Pos_final):-
    ordena_poss(Poss,Poss_ord, [],Pos_inicial,Pos_final).
ordena_poss([],Poss_ord_aux,Poss_ord_aux,_,_).
ordena_poss(Poss, Poss_ord, Poss_ord_aux, Pos_inicial, Pos_final):-
    min(Poss, Min, Pos_inicial, Pos_final),
	remove_elemento(Poss, Min, NPoss), 
	append(Poss_ord_aux, Min, NPoss_ord_aux),
	ordena_poss(NPoss, Poss_ord, NPoss_ord_aux,
                Pos_inicial, Pos_final ).
	
posicao((_,X,Y), (X,Y)).

resolve1(Lab,(IX,IY),F,Movs) :- 
    resolve1(Lab,(IX,IY),F,Movs,[(i,IX,IY)],(IX,IY)).
resolve1(_, _, F, Movs_aux, Movs_aux,F) :- !.
resolve1(Lab,_,_, _, Movs_aux,Pos):-
    movs_possiveis(Lab, Pos, Movs_aux, Poss),
    Poss == [],
    fail.
resolve1(Lab,I,F, Movs, Movs_aux,Pos):-
    movs_possiveis(Lab, Pos, Movs_aux, Poss),
    member(Mov,Poss),
    append(Movs_aux,Mov,Nmovs_aux),
    posicao(Mov,NPos),
    resolve1(Lab,I,F,Movs,Nmovs_aux,NPos).
    
resolve2(Lab,(IX,IY),F,Movs) :- 
    resolve2(Lab,(IX,IY),F,Movs,[(i,IX,IY)],(IX,IY)).
resolve2(_, _, F, Movs_aux, Movs_aux,F) :- !.
resolve2(Lab,_,_, _, Movs_aux,Pos):-
    movs_possiveis(Lab, Pos, Movs_aux, Poss),
    Poss == [],
    fail.
resolve2(Lab,I,F, Movs, Movs_aux,Pos):-
    movs_possiveis(Lab, Pos, Movs_aux, Poss),
    ordena_poss(Poss,Poss_ord,I,F),
    member(Mov,Poss_ord),
    append(Movs_aux,Mov,Nmovs_aux),
    posicao(Mov,NPos),
    resolve2(Lab,I,F,Movs,Nmovs_aux,NPos).  
  





    
