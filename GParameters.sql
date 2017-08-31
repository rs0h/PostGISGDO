create table gdo.gparameters(
gparameter varchar(63) not null primary key,
gvalue varchar(255) not null);
insert into gdo.galiastable values ('GParameters', 'gdo.gparameters');
