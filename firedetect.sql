create table sensor(
	id int auto_increment primary key, 
	sensor_name varchar(100),
    analog_value int,
    digital_value boolean
);
alter table sensor add rgb_control varchar(20);

select * from sensor;

/* 센서마다 Table 생성*/

create table temperature(
	id int auto_increment primary key,
    value int
);

