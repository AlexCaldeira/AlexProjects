----------------------------------------
-- Eliminacao das Tabelas
----------------------------------------

drop table if exists camara cascade;
drop table if exists video cascade;
drop table if exists segmentodevideo cascade;
drop table if exists local cascade;
drop table if exists vigia cascade;
drop table if exists eventoemergencia cascade;
drop table if exists processosocorro cascade;
drop table if exists entidademeio cascade;
drop table if exists meio cascade;
drop table if exists meiocombate cascade;
drop table if exists meioapoio cascade;
drop table if exists meiosocorro cascade;
drop table if exists transporta cascade;
drop table if exists alocado cascade;
drop table if exists acciona cascade;
drop table if exists coordenador cascade;
drop table if exists audita cascade;
drop table if exists solicita cascade;

----------------------------------------
-- Criacao das Tabelas
----------------------------------------

-- Named constraints are global to the database.
-- Therefore the following use the following naming rules:
--   1. pk_table for names of primary key constraints
--   2. fk_table_another for names of foreign key constraints
--   3. u_table for names of unique keys
--   4. ck_table for check constraints

-- Camara
create table camara(
  num_camara   integer not null,
  constraint pk_camara primary key(num_camara));

--Video
create table video(
  data_hora_inicio  timestamp not null,
  data_hora_fim   timestamp not null,
  num_camara  integer not null ,
  constraint pk_video primary key(data_hora_inicio, num_camara) ,
  constraint fk_video_camara foreign key(num_camara) references camara(num_camara)ON DELETE CASCADE ON UPDATE CASCADE);

-- Segmento De Video
create table segmentodevideo(
  num_segmento  integer not null,
  duracao   real not null,
  data_hora_inicio  timestamp not null,
  num_camara  integer not null,
  constraint pk_segmentodevideo primary key(num_segmento, data_hora_inicio, num_camara),
  constraint fk_segmentodevideo_video foreign key(data_hora_inicio, num_camara) references video(data_hora_inicio, num_camara)ON DELETE CASCADE ON UPDATE CASCADE);

--
create table local(
  morada  varchar(255) not null,
  constraint pk_local primary key(morada));

-- Vigia erro na morada
create table vigia(
  num_camara   integer not null,
  morada   varchar(255) not null,
  constraint pk_vigia primary key(morada, num_camara),
  constraint fk_vigia_local foreign key(morada) references local(morada),
  constraint fk_vigia_camara foreign key(num_camara) references camara(num_camara)ON DELETE CASCADE ON UPDATE CASCADE);


create table processosocorro(
  num_processo_socorro   integer not null,
  constraint pk_processosocorro primary key(num_processo_socorro));

-- EventoEmergencia
create table eventoemergencia(
  num_telefone   char(9) not null,
  instante_chamada   timestamp not null,
  nome_pessoa   varchar(255) not null,
  morada   varchar(255) not null,
  num_processo_socorro   integer,
  constraint u_eventoemergencia unique(num_telefone, nome_pessoa),
  constraint pk_eventoemergencia primary key(num_telefone, instante_chamada),
  constraint fk_eventoemergencia_local foreign key(morada) references local(morada)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_eventoemergencia_processosocorro foreign key(num_processo_socorro) references processosocorro(num_processo_socorro)ON DELETE CASCADE ON UPDATE CASCADE);


-- Entidade Meio
create table entidademeio(
  nome_entidade   varchar(255) not null,
  constraint pk_entidademeio primary key(nome_entidade));


--Meio
create table meio(
  num_meio  integer,
  nome_meio   varchar(255),
  nome_entidade   varchar(255),
  constraint u_meio unique(num_meio, nome_entidade),
  constraint pk_meio primary key(num_meio),
  constraint fk_meio_entidademeio foreign key(nome_entidade) references entidademeio(nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE);

--Meio Combate
create table meiocombate(
  num_meio  integer,
  nome_entidade   varchar(255),

  constraint pk_meiocombate primary key(num_meio, nome_entidade),
  constraint fk_meiocombate_meio foreign key(num_meio, nome_entidade) references meio(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE);


--Meio Apoio
create table meioapoio(
  num_meio  integer,
  nome_entidade   varchar(255),
  constraint pk_meioapoio primary key(num_meio, nome_entidade),
  constraint fk_meioapoio_meio foreign key(num_meio, nome_entidade) references meio(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE);


--Meio Socorro
create table meiosocorro(
  num_meio  integer,
  nome_entidade   varchar(255),
  constraint pk_meiosocorro primary key(num_meio, nome_entidade),
  constraint fk_meiosocorro_meio foreign key(num_meio, nome_entidade) references meio(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE);


--Transporta
create table transporta(
  num_meio  integer not null,
  nome_entidade   varchar(255) not null,
  num_vitimas   integer,
  num_processo_socorro  integer not null,
  constraint pk_transporta primary key(num_meio, nome_entidade, num_processo_socorro),
  constraint fk_transporta_meiosocorro foreign key(num_meio, nome_entidade) references meiosocorro(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_transporta_processosocorro foreign key(num_processo_socorro) references processosocorro(num_processo_socorro)ON DELETE CASCADE ON UPDATE CASCADE);


--Alocado
create table alocado(
  num_meio  integer not null,
  nome_entidade   varchar(255) not null,
  num_horas   integer not null,
  num_processo_socorro  integer,
  constraint pk_alocado primary key(num_meio, nome_entidade, num_processo_socorro),
  constraint fk_alocado_meioapoio foreign key(num_meio, nome_entidade) references meioapoio(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_alocado_processosocorro foreign key(num_processo_socorro) references processosocorro(num_processo_socorro)ON DELETE CASCADE ON UPDATE CASCADE);


--Acciona
create table acciona(
  num_meio   integer not null,
  nome_entidade   varchar(255) not null,
  num_processo_socorro   integer,
  constraint pk_acciona primary key(num_meio, nome_entidade, num_processo_socorro),
  constraint fk_acciona_meio foreign key(num_meio, nome_entidade) references meio(num_meio, nome_entidade)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_acciona_processosocorro foreign key(num_processo_socorro) references processosocorro(num_processo_socorro)ON DELETE CASCADE ON UPDATE CASCADE);


--Coordenador
create table coordenador(
  id_coordenador  integer not null,
  constraint pk_coordenador primary key(id_coordenador));


-- Audita
create table audita(
  id_coordenador   integer not null,
  num_meio   integer not null,
  nome_entidade    varchar(255) not null,
  num_processo_socorro   integer not null,
  data_hora_inicio   timestamp not null,
  data_hora_fim    timestamp not null,
  data_auditoria   date not null,
  texto    text not null,
  constraint ck_audita_hora check(data_hora_inicio < data_hora_fim),
  constraint ck_data_audita check(data_auditoria <= now()),
  constraint pk_audita primary key(id_coordenador, num_meio, nome_entidade, num_processo_socorro),
  constraint fk_audita_acciona foreign key(num_meio, nome_entidade, num_processo_socorro) references acciona(num_meio, nome_entidade, num_processo_socorro)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_audita_coordenador foreign key(id_coordenador) references coordenador(id_coordenador)ON DELETE CASCADE ON UPDATE CASCADE);

--Solicita
create table solicita(
  id_coordenador   integer not null,
  data_hora_inicio_video   timestamp not null,
  num_camara   integer not null,
  data_hora_inicio   timestamp not null,
  data_hora_fim    timestamp not null,
  constraint ck_solicita_hora check( data_hora_inicio < data_hora_fim),
  constraint ck_solicita_hora_video check( data_hora_inicio_video < data_hora_inicio),
  constraint pk_solicita primary key(id_coordenador, data_hora_inicio_video, num_camara),
  constraint fk_solicita_coordenador foreign key(id_coordenador) references coordenador(id_coordenador)ON DELETE CASCADE ON UPDATE CASCADE,
  constraint fk_solicita_video foreign key(data_hora_inicio_video, num_camara) references video(data_hora_inicio, num_camara)ON DELETE CASCADE ON UPDATE CASCADE);

--create trigger after delete on processosocorro
