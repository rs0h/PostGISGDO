create table gdo_graphic_columns(
f_table_schema name not null,
f_table_name name not null,
f_graphic_column name not null,
coord_dimension integer not null,
srid integer not null,
type name not null,
f_native_geom_column name,
f_plain_label_column name,
primary key (f_table_schema, f_table_name, f_graphic_column))
