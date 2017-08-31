delete from gdo.gfeaturesbase where not
(featurename in (select schemaname || '.' || tablename from pg_tables) or
featurename in (select tablename from pg_tables where schemaname = 'public') or
featurename in (select schemaname || '.' || viewname from pg_views) or
featurename in (select viewname from pg_views where schemaname = 'public'));

drop view gdo.gfeatures;

create view gdo.gfeatures as select ctid, * from gdo.gfeaturesbase where
featurename in (select table_schema || '.' || table_name from information_schema.tables) or
featurename in (select table_name from information_schema.tables where table_schema = 'public') or not
(featurename in (select schemaname || '.' || tablename from pg_tables) or
featurename in (select tablename from pg_tables where schemaname = 'public') or
featurename in (select schemaname || '.' || viewname from pg_views) or
featurename in (select viewname from pg_views where schemaname = 'public'));

create rule gfeatures_upd as on update to gdo.gfeatures
do instead update gdo.gfeaturesbase set featurename = new.featurename,
geometrytype = new.geometrytype, primarygeometryfieldname = new.primarygeometryfieldname,
featuredescription = new.featuredescription where gdo.gfeaturesbase.ctid = old.ctid
returning gdo.gfeaturesbase.ctid, gdo.gfeaturesbase.*;

create rule gfeatures_ins as on insert to gdo.gfeatures
do instead insert into gdo.gfeaturesbase values (new.featurename,
new.geometrytype, new.primarygeometryfieldname, new.featuredescription)
returning gdo.gfeaturesbase.ctid, gdo.gfeaturesbase.*;

create rule gfeatures_del as on delete to gdo.gfeatures
do instead delete from gdo.gfeaturesbase where gdo.gfeaturesbase.ctid = old.ctid;
