--SQL 1###########################################################################################
--Qual o processo de socorro que envolveu maior numero de meios distintos

SELECT num_processo_socorro, SUM(n_meios_por_entidade) AS n_meios 
FROM (
		SELECT num_processo_socorro, nome_entidade, count(DISTINCT num_meio)
		from acciona
		group by num_processo_socorro, nome_entidade
	  ) AS table1(num_processo_socorro, nome_entidade, n_meios_por_entidade)
GROUP BY num_processo_socorro
ORDER BY n_meios DESC
LIMIT 1;



--SQL 2#######################################################################################
--Qual a entidade que participou em mais processos no verão de 2018

SELECT nome_entidade, mycount
FROM(
	SELECT nome_entidade, COUNT(DISTINCT num_processo_socorro)
	FROM eventoemergencia NATURAL JOIN acciona
	WHERE instante_chamada >= '2018-06-21 00:00:00' AND instante_chamada <= '2018-09-23 00:00:00'
	GROUP BY nome_entidade
	)AS table1(nome_entidade, mycount)
GROUP BY nome_entidade, mycount
ORDER BY mycount DESC
LIMIT 1
;



--SQL 3#####################################################################################################################
--Os processos de socorro em Oliveira do Hospital de 2018, onde existe pelo menos um acionamento de meios que não foi auditado

SELECT num_processo_socorro
FROM eventoemergencia
WHERE morada = 'Oliveira do Hospital' 
AND instante_chamada >= '2018-06-21 00:00:00' AND instante_chamada <= '2018-09-23 00:00:00'
AND num_processo_socorro IN (
	SELECT acciona.num_processo_socorro
	FROM acciona LEFT OUTER JOIN audita
	ON acciona.num_Meio = audita.num_meio AND acciona.nome_entidade = audita.nome_entidade
	WHERE audita.num_processo_socorro IS NULL
);


--SQL 4###################################################################################################################
--Quantos segmentos foram gravados com mais que 60 segundos em Monchique durante o Agosto de 2018

SELECT COUNT(num_segmento)
FROM segmentodevideo NATURAL JOIN vigia
WHERE morada = 'Monchique'
AND data_hora_inicio >= '2018-08-01 00:00:00' 
AND data_hora_inicio <= '2018-08-31 00:00:00'
AND duracao > 60;


--SQL 5###################################################################################################################
--Liste meios de combate que não foram usados como meios de apoio em nenhum processo de socorro

(SELECT num_meio, nome_entidade FROM meiocombate NATURAL JOIN acciona)
EXCEPT
(SELECT num_meio, nome_entidade FROM meiosocorro NATURAL JOIN acciona)


--SQL 6#####################################################################################################################
--Liste as entidades que forneceram meios de combate a todos os Processos de socorro que acionaram meios

SELECT DISTINCT nome_entidade
FROM acciona
WHERE nome_entidade IN(
	SELECT nome_entidade
	FROM acciona NATURAL JOIN meiocombate
	GROUP BY nome_entidade
	HAVING COUNT(DISTINCT num_processo_socorro) = (
		SELECT COUNT(DISTINCT num_processo_socorro)
		FROM acciona)
);


